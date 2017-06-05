/*
 * Copyright (C) 2017 francisco miguel garcia rodriguez
 * Contact: http://www.dmious.com/qe/licensing/
 *
 * This file is part of the QE Common module of the QE Toolkit.
 *
 * $QE_BEGIN_LICENSE:LGPL21$
 * Commercial License Usage
 * Licensees holding valid commercial QE licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and The Dmious Company. For licensing terms
 * and conditions see http://www.dmious.com/qe/terms-conditions. For further
 * information use the contact form at http://www.dmious.com/contact-us.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 3 as published by the Free
 * Software Foundation and appearing in the file LICENSE.LGPLv3
 * included in the packaging of this file. Please review the
 * following information to ensure the GNU Lesser General Public License
 * requirements will be met: https://www.gnu.org/licenses/lgpl.html and
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 * $QE_END_LICENSE$
 *
 */

#include "EntityDefPrivate.hpp"
#include "ModelRepository.hpp"
#include <qe/common/Exception.hpp>
#include <qe/entity/Model.hpp>
#include <QStringBuilder>

using namespace qe::entity;
using namespace qe::common;
using namespace std;

namespace {
#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
       EntityDef::MappingType toMappingType( const QString& mappingTypeStr, bool *enumOk)
       {
               static vector<QString> strValues = { "NoMappingType", "OneToOne", "OneToMany", "ManyToOne", "ManyToMany" };
               EntityDef::MappingType mt = EntityDef::MappingType::NoMappingType;
               auto itr = find( begin(strValues), end(strValues), mappingTypeStr);
               if( enumOk )
                       *enumOk = (itr != end(strValues));

               if( itr != end(strValues))
                       mt = static_cast<EntityDef::MappingType>( distance( begin(strValues), itr));

               return mt;
       }
#endif

	/// \brief It checks if property @p propertyName requires a OneToMany
	/// mapping.
	/// \internal Right now, only \c StringList type return true.
	bool typeRequiresOneToManyMapping(
		const QMetaObject* mo,
		const QByteArray &propertyName )
	{
		const int propIndex = mo->indexOfProperty( propertyName.constData());
		const QMetaProperty metaProp = mo->property( propIndex);
		const QVariant::Type propType = metaProp.type();

		return propType == QVariant::Type::StringList;
	}

	EntityDef::MappedType decodeOneToManyRelation(
		const qe::annotation::Model & model,
		const QString& propName)
	{
		bool enumOk;
		QString mappedTypeStr;

		if( typeRequiresOneToManyMapping( model.metaObject(), propName.toUtf8()))
			mappedTypeStr = QStringLiteral( "OneToMany");
		else
			mappedTypeStr = model.annotation( propName,
				qe::entity::tags::mappingType()).value( QStringLiteral("NoMappingType"))
			.toString();

#if ( QT_VERSION >= QT_VERSION_CHECK( 5, 5, 0))
		EntityDef::MappedType mappedType = static_cast<EntityDef::MappedType>(
			QMetaEnum::fromType< EntityDef::MappedType>()
				.keyToValue(
					mappedTypeStr.toUtf8().constData(),
					&enumOk));
#else
		EntityDef::MappedType mappedType = toMappedType( mappedTypeStr, &enumOk);
#endif

		if( !enumOk)
			Exception::makeAndThrow(
				QStringLiteral( "QE Entity cannot decode mapping type '%1' on property '%2'")
					.arg( mappedTypeStr)
					.arg( propName));

		return mappedType;
	}

	shared_ptr<Model> createOneToManyForSimpleTypes(
		const QString& modelName,
		const QByteArray& propertyName,
		const QVariant::Type type)
	{
		EntityDef value( propertyName, type);
		EntityDef key( "id", QVariant::Type::Int);
		key.setAutoIncrement();

		shared_ptr<Model> m = make_shared<Model>(
			modelName,
			EntityDefList{ key, value});

		return m;
	}
}

/**
 * \class EntityDefPrivate
 * \since 1.0.0
 */

EntityDefPrivate::EntityDefPrivate(
	const QByteArray& property,
	const int type,
	const uint maxLength,
	const qe::entity::Model *model)
	: propertyName( property),
	propertyType( type),
	maxLength( maxLength)
{
	if( model)
	{
		decodeProperties( *model);

		try
		{
			decodeOneToManyRelations( *model);
		}
		catch ( const common::Exception & error )
		{
			qCWarning( lcEntityDef) << error.what();
			mappedType = EntityDef::MappedType::NoMappedType;
		}
	}

	if( type == QMetaType::Char
			|| type == QMetaType::QChar
			|| type == QMetaType::SChar
			|| type == QMetaType::UChar)
		this->maxLength = 1;
}

EntityDefPrivate::EntityDefPrivate(
	const QByteArray& property,
	const QMetaEnum me,
	const qe::entity::Model *model)
	: EntityDefPrivate( property, QMetaType::Int, 0, model)
{
	metaEnum = me;
}

void EntityDefPrivate::decodeProperties(const Model &model)
{
	entityName = model.annotation( propertyName,
		qe::entity::tags::entityName()).value( propertyName).toString();

	isNullable = model.annotation( propertyName,
			tags::isNullable()).value( true).toBool();

	isAutoIncrement = model.annotation( propertyName,
			tags::isAutoIncrementable()).value( false).toBool();

	maxLength = model.annotation( propertyName,
			tags::entityMaxLength()).value( 0).toUInt();
}

void EntityDefPrivate::decodeOneToManyRelations(const qe::entity::Model &model)
{
	QString mappingEntityName = model.annotation( propertyName,
				  tags::mappingEntity()).value().toString();
	mappedType = decodeOneToManyRelation( model, propertyName);

	// By default use OneToMany mapping type when mapping Entity is used.
	if( ! mappingEntityName.isEmpty()
			&& mappedType == EntityDef::MappedType::NoMappedType)
		mappedType = EntityDef::MappedType::OneToMany;

	if( mappedType != EntityDef::MappedType::NoMappedType)
	{
		if( ! mappingEntityName.isEmpty())
		{
			const QString classPointer = mappingEntityName + QChar('*');
			int typeId = QMetaType::type( classPointer.toLocal8Bit());
			if( typeId == QMetaType::UnknownType)
				Exception::makeAndThrow(
					QStringLiteral("QE Entity cannot map entity of type '")
					% mappingEntityName
					% QStringLiteral( "'. Please use 'Q_DECLARE_METATYPE' and 'qRegisterMetaType' to add entity to meta-object system."));

			mappedModel = ModelRepository::instance().model( QMetaType::metaObjectForType( typeId));
		}
		else
		{
			mappingEntityName = model.name() % "_" % propertyName;
			const auto typeId = QVariant::Type::String;
			mappedModel = createOneToManyForSimpleTypes(
				mappingEntityName,
				propertyName,
				typeId);
		}
	}
}

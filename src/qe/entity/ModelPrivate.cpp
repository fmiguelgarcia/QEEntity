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

#include "ModelPrivate.hpp"
#include "EntityDef.hpp"
#include <QStringBuilder>
#include <iterator>

using namespace qe::entity;
using namespace qe::common;
using namespace std;

namespace {

	inline
	Model modelFromPrivate( ModelPrivate* mp)
	{
		return Model(
			QExplicitlySharedDataPointer<ModelPrivate> ( mp));
	}

	QString classId() noexcept
	{ return QStringLiteral("class");}

	/// @brief It checks if property is register into Qt metatype system.
	bool isPropertyTypeRegister( const QMetaObject* mo, const QMetaProperty& property)
	{
		const int propType = property.userType();
		const bool isRegistered = QMetaType::isRegistered( propType);

		if( propType == QMetaType::Type::UnknownType ||
			! isRegistered)
		{
			qCritical() << QString ("Property %1:%2 is not register and will be ignore."
				"Please use QMetaType::qRegisterMetaType and Q_DECLARE_METATYPE")
				.arg( mo->className()).arg( property.name());
		}

		return propType != QMetaType::Type::UnknownType
			&& isRegistered;
	}

	/// @brief It gets the primary keys from annotation.
	///
	/// If there is no explicit primary key, it will use the first
	/// 'auto_increment' file as a primary key.
	/// If there is not primary key neither 'auto_increment', then
	/// all en
	EntityDefList parsePrimaryKeys( const qe::entity::ModelPrivate & model)
	{
		EntityDefList pk;
		QStringList pkPropertyNames = model.findAnnotation(
				classId(),
				tags::primaryKey())
			.value( QString()).toString()
			.split( ',', QString::SkipEmptyParts);

		if( pkPropertyNames.isEmpty())
		{
			// If empty, try 'auto_increment' entity.
			const auto colDef = model.findEntityDef( FindEntityDefByAutoIncrement{});
			if( colDef)
				pkPropertyNames << colDef->propertyName();
		}

		for( QString pkPropName: pkPropertyNames)
		{
			const auto colDef = model.findEntityDef(
					FindEntityDefByPropertyName{ pkPropName.toLocal8Bit()});

			if( colDef )
				pk.push_back( *colDef);
		}

		// If it is still empty, use all entities as primary key.
		if( pk.empty())
			pk = model.entityDefs;

		return pk;
	}
}

ModelPrivate::ModelPrivate(
	const QMetaObject* meta)
	: qe::annotation::ModelPrivate( meta)
{}

ModelPrivate::ModelPrivate (
	const QString & name,
	const EntityDefList& entities)
	: qe::annotation::ModelPrivate( nullptr),
	name( name),
	entityDefs( entities)
{
	setPrimaryKey( parsePrimaryKeys( *this));
}

void ModelPrivate::parseAnnotations( const QMetaObject* metaObj)
{
	if( metaObj)
	{
		// Class annotation
		name = findAnnotation(
				classId(),
				qe::entity::tags::modelName())
			.value( QString( metaObj->className()))
			.toString();

		// Entity
		const bool exportParents = findAnnotation(
					classId(),
					qe::entity::tags::isParentExported())
				.value( false).toBool();

		const int begin = (exportParents)
			? 0
			: metaObj->propertyOffset();

		for( int i = begin; i < metaObj->propertyCount(); ++i)
		{
			parseAnnotation(
				metaObj,
				metaObj->property(i));
		}
	}

	setPrimaryKey( parsePrimaryKeys( *this));
}

void ModelPrivate::parseAnnotation(
	const QMetaObject* metaObj,
	const QMetaProperty& property)
{
	if( isPropertyTypeRegister( metaObj, property))
	{
		const QByteArray propertyName = property.name();
		const bool isEnable = findAnnotation(
				propertyName,
				qe::entity::tags::isEnabled())
			.value( true).toBool();

		if( isEnable )
		{
			Model model = modelFromPrivate( this);

			if( property.isEnumType())
				entityDefs.emplace_back(
					propertyName,
					property.enumerator(),
					model);
			else
				entityDefs.emplace_back(
					propertyName,
					property.type(),
					0,
					model);
		}
	}
}

optional<EntityDef> ModelPrivate::findEntityDef(
	const FindEntityDefByAutoIncrement& ) const noexcept
{
	return findEntityDef(
		[]( const EntityDef& colDef) -> bool
		{
			return
				colDef.isAutoIncrement()
				&& colDef.mappedType() == EntityDef::MappedType::NoMappedType;
		});
}

optional<EntityDef> ModelPrivate::findEntityDef(
	const FindEntityDefByPropertyName& property) const noexcept
{
	return findEntityDef(
		[&property]( const EntityDef& eDef) -> bool
		{ return eDef.propertyName() == property.name;});
}

optional<EntityDef> ModelPrivate::findEntityDef(
	const FindEntityDefByEntityName& entity) const noexcept
{
	return findEntityDef(
		[&entity]( const EntityDef& colDef) -> bool
		{ return colDef.entityName() == entity.name;});
}

optional<EntityDef> ModelPrivate::findEntityDef(
	EntityDefPredictate&& predicate) const noexcept
{
	optional<EntityDef> eDef;
	auto begin = std::begin( entityDefs);
	auto end = std::end( entityDefs);

	const auto itr = find_if( begin, end, std::move(predicate));
	if( itr != end)
		eDef = *itr;

	return eDef;
}

const EntityDefList& ModelPrivate::primaryKey() const noexcept
{ return m_primaryKeyDef; }

void ModelPrivate::setPrimaryKey( const EntityDefList& pk)
{
	m_primaryKeyDef = pk;

	// Update ManyToOne references.
	for( EntityDef& eDef : entityDefs)
	{
		if( eDef.mappedType() == EntityDef::MappedType::OneToMany)
		{
			optional<Model> subModel = eDef.mappedModel();
			if( subModel)
			{
				Model model = modelFromPrivate( this);

				subModel->addReferenceManyToOne(
					eDef.propertyName(),
					model);
			}
		}
	}
}

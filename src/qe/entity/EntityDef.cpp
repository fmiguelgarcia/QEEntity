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
 *
 * $QE_END_LICENSE$
 */

#include "EntityDef.hpp"
#include <qe/common/Exception.hpp>
#include <qe/entity/Model.hpp>
#include <QMetaEnum>
#include <QStringBuilder>
#include <utility>

using namespace qe;
using namespace qe::entity;
using namespace qe::annotation;
using namespace std;
Q_LOGGING_CATEGORY( qe::entity::lcEntityDef, "com.dmious.qe.entity.EntityDef")

EntityDef::EntityDef( const QByteArray &propertyName, const int propertyType,
				const QString& entityName, const uint entityMaxLength)
	: m_propertyName( propertyName), m_propertyType( propertyType),
	m_entityName( entityName), m_entityMaxLength( entityMaxLength)
{}

EntityDef::EntityDef( const QByteArray &property, const int type,
	  	const qe::annotation::Model &model)
	: m_propertyName(property), m_propertyType( type)
{
	decodeProperties( model);
	
	try {
		decodeMapping( model);
	} catch ( const common::Exception & error ){
		qCWarning( lcEntityDef) << error.what();
		m_mappingType = MappingType::NoMappingType;
	}

	if( type == QMetaType::Char 
		|| type == QMetaType::QChar
		|| type == QMetaType::SChar
		|| type == QMetaType::UChar)
		m_entityMaxLength = 1;
}

void EntityDef::decodeProperties(const qe::annotation::Model &model)
{
	const QString propName = propertyName();
	m_entityName = model.annotation( propName, 
		qe::entity::tags::entityName()).value( propName).toString();

	m_isNullable = model.annotation( propName, 
			tags::isNullable()).value( true).toBool();

	m_isAutoIncrement = model.annotation( propName, 
			tags::isAutoIncrementable()).value( false).toBool();

	m_entityMaxLength = model.annotation( propName, 
			tags::entityMaxLength()).value( 0).toUInt();
}

EntityDef::MappingType decodeMappingType( const qe::annotation::Model & model, const QString& propName)
{
	bool enumOk;
	const QString mappingTypeStr = model.annotation( propName, 
		qe::entity::tags::mappingType()).value( QStringLiteral("NoMappingType"))
			.toString();

	EntityDef::MappingType mappingType = static_cast<EntityDef::MappingType>( 
		QMetaEnum::fromType< EntityDef::MappingType>()
			.keyToValue( 
				mappingTypeStr.toLocal8Bit().constData(), 
				&enumOk)); 

	if( !enumOk)
		common::Exception::makeAndThrow(
			QStringLiteral( "QE Entity cannot decode mapping type '%1' on property '%2'")
				.arg( mappingTypeStr)
				.arg( propName));

	return mappingType;
}

void EntityDef::decodeMapping(const qe::annotation::Model &model)
{
	const QString propName = propertyName();
	const QString mappingEntityName = model.annotation( propName, 
				  tags::mappingEntity()).value().toString();
	m_mappingType = decodeMappingType( model, propName);
	
	// By default use OneToMany mapping type when mapping Entity is used.
	if( ! mappingEntityName.isEmpty() && m_mappingType == MappingType::NoMappingType)
		m_mappingType = MappingType::OneToMany;
	
	if( m_mappingType != MappingType::NoMappingType)
	{
		if( mappingEntityName.isEmpty())
			common::Exception::makeAndThrow(
				QStringLiteral("QE Entity requires no empty mapping entity on property '%1'")
					.arg( propName));
		
		// Check metadata is available.
		const QString classPointer = mappingEntityName + QChar('*');
		const int typeId = QMetaType::type( classPointer.toLocal8Bit());
		if( typeId == QMetaType::UnknownType)
			common::Exception::makeAndThrow(
				QStringLiteral("QE Entity cannot map entity of type '") 
				% mappingEntityName 
				% QStringLiteral( "'. Please use 'Q_DECLARE_METATYPE' and 'qRegisterMetaType' to add entity to meta-object system."));
		
		m_mappingEntity = QMetaType::metaObjectForType( typeId);
	}
}


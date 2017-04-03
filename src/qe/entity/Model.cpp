/*
 * Copyright (C) 2017 Francisco Miguel García Rodríguez
 * Contact: http://www.dmious.com/qe/licensing/
 *
 * This file is part of the QE Common module of the QE Toolkit.
 *
 * $QE_BEGIN_LICENSE$
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
 * General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL3 included in the
 * packaging of this file. Please review the following information to
 * ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
 *
 * $QE_END_LICENSE$
 */
#include "Model.hpp"
#include "EntityDef.hpp"
#include "RelationDef.hpp"
#include <QMetaProperty>
#include <QStringBuilder>
#include <utility>
#include <memory>

using namespace qe::entity;
using namespace std;
Q_LOGGING_CATEGORY( qe::entity::lcModel,
	"com.dmious.ipmo.qe.entity.model");

namespace {
	
	QString classId() noexcept
	{ return QStringLiteral("class");}

	/// @brief It gets the primary keys from annotation.
	/// 
	/// If there is no explicit primary key, it will use the first
	/// 'auto_increment' file as a primary key.
	/// If there is not primary key neither 'auto_increment', then
	/// all en
	EntityDefList parsePrimaryKeys( const Model & model)
	{
		EntityDefList pk;
		QStringList pkPropertyNames = model.annotation( 
				classId(),
				tags::primaryKey())
			.value( QString()).toString()
			.split( ',', QString::SkipEmptyParts);

		if( pkPropertyNames.isEmpty())
		{
			// If empty, try 'auto_increment' entity.
			EntityDefShd colDef = model.findEntityDef( Model::findByAutoIncrement{});
			if( colDef)
				pkPropertyNames << colDef->propertyName();
		}

		for( QString pkPropName: pkPropertyNames)
		{
			EntityDefShd colDef = model.findEntityDef( 
					Model::findByPropertyName{ pkPropName.toLocal8Bit()});
			
			if( colDef )
				pk.push_back( colDef);
		}
		
		// If it is still empty, use all entities as primary key.
		if( pk.empty())
			pk = model.entityDefs();

		return pk;
	}

	/// @brief It checks if property is register into Qt metatype system.
	bool isPropertyTypeRegister( const QMetaObject* mo, const QMetaProperty& property)
	{
		const int propType = property.userType();
		const bool isRegistered = QMetaType::isRegistered( propType);
		
		if( propType == QMetaType::Type::UnknownType ||
			! isRegistered)
		{
			qCCritical( lcModel) << QString ("Property %1:%2 is not register and will be ignore."
				"Please use QMetaType::qRegisterMetaType and Q_DECLARE_METATYPE")
				.arg( mo->className()).arg( property.name());
		}
		
		return propType != QMetaType::Type::UnknownType
			&& isRegistered; 
	}
}

// Class Model
// ============================================================================

Model::Model( const QMetaObject* metaObj)
	: qe::annotation::Model( metaObj) 
{
	parseAnnotations( metaObj);
}

const QString& Model::name() const noexcept
{ return m_name; }

const EntityDefList& Model::entityDefs() const noexcept
{ return m_entityDefs; }

const EntityDefList& Model::primaryKeyDef() const noexcept
{ return m_primaryKeyDef; }

const RelationDefList & Model::referencesManyToOneDefs() const noexcept
{ return m_referencesManyToOneDefs; }

void Model::addReferenceManyToOne( const QByteArray& propertyName, 
		const ModelShd &reference)
{
	RelationDefShd fkDef = make_shared<RelationDef>( propertyName, reference);

	// Fix fk column names to avoid duplicates.
	for( auto & fkColDef: fkDef->relationKey())
	{
		QString fkColName = fkColDef->entityName();
		auto colDef = findEntityDef( findByEntityName{ fkColName });
		if( colDef )
		{
			uint colissionIdx = 0;
			fkColName = reference->name() 
				% QStringLiteral("_") 
				% fkColDef->entityName(); 

			colDef =  findEntityDef( findByEntityName{ fkColName });
			while( colDef )
			{
				fkColName = QString("%1_%2_%3").arg( reference->name())
					.arg( fkColDef->entityName()).arg( ++colissionIdx);
				colDef = findEntityDef( findByEntityName{ fkColName });
			}
		}
		
		// Update RelationDef 
		fkColDef->setEntityName( fkColName);
		fkColDef->setMappingType( EntityDef::MappingType::ManyToOne);
		m_entityDefs.push_back( fkColDef);
	}

	// Copy ref
	m_referencesManyToOneDefs.push_back( fkDef);
}

void Model::parseAnnotations( const QMetaObject* metaObj)
{
	// Class annotation 
	m_name = annotation( classId(), 
		qe::entity::tags::modelName())
			.value( QString( metaObj->className())).toString();

	// Entity 
	const bool exportParents = annotation( classId(),
		  qe::entity::tags::isParentExported())
		.value( false).toBool();
	const int begin = (exportParents) ? 0 : metaObj->propertyOffset();

	for( int i = begin; i < metaObj->propertyCount(); ++i)
	{
		const QMetaProperty property = metaObj->property(i);
		if( isPropertyTypeRegister( metaObj, property))
		{
			const QByteArray propertyName = property.name();
			const bool isEnable = annotation( propertyName, 
											  qe::entity::tags::isEnabled())
			.value( true).toBool();
			
			if( isEnable )
			{
				EntityDefShd colDef;

				if( property.isEnumType())
					colDef = make_shared<EntityDef>( propertyName, 
						property.enumerator(), *this);
				else
					colDef = make_shared<EntityDef>( propertyName, 
						property.type(), *this);

				m_entityDefs.push_back( colDef);
			}
		}
	}
	
	m_primaryKeyDef = parsePrimaryKeys( *this);
}

EntityDefShd Model::findEntityDef( Model::FindColDefPredicate&& predicate) const noexcept
{
	EntityDefShd column;
	const auto itr = find_if( begin( m_entityDefs), end( m_entityDefs), predicate);
	if( itr != end( m_entityDefs))
		column = *itr;

	return column;
}

EntityDefShd Model::findEntityDef(const Model::findByPropertyName& property) const noexcept
{
	return findEntityDef( 
		[&property]( const EntityDefShd& eDef) -> bool 
			{ return eDef->propertyName() == property.name;});
}

EntityDefShd Model::findEntityDef(const Model::findByEntityName& entity) const noexcept
{
	return findEntityDef( 
		[&entity]( const EntityDefShd& colDef) -> bool
			{ return colDef->entityName() == entity.name;});
}

EntityDefShd Model::findEntityDef( const Model::findByAutoIncrement& ) const noexcept
{
	return findEntityDef( 
		[]( const EntityDefShd& colDef) -> bool
			{ return colDef->isAutoIncrement();});
}

const RelationDefShd Model::findRelationTo( const ModelShd& model) const noexcept
{
	RelationDefShd fk;

	const auto itr = find_if( 
		begin( m_referencesManyToOneDefs), 
		end( m_referencesManyToOneDefs),
		[model]( const RelationDefShd& fkDef) -> bool
			{ return fkDef->reference() == model;});

	if( itr != end( m_referencesManyToOneDefs))
		fk = *itr;

	return fk;
}


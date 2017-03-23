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
namespace {

	/// @brief It gets the primary keys from annotation.
	/// 
	/// If there is no explicit primary key, it will use the first
	/// 'auto_increment' file as a primary key.
	EntityDefList parsePrimaryKeys( const Model & model)
	{
		EntityDefList pk;
		QStringList pkPropertyNames = model.annotation( 
				QStringLiteral("class"),
				tags::primaryKey())
			.value( QString()).toString()
			.split( ',', QString::SkipEmptyParts);

		if( pkPropertyNames.isEmpty())
		{
			EntityDefShd colDef = model.findEntityDef( Model::findByAutoIncrement{});
			if( colDef)
				pkPropertyNames << colDef->propertyName();
		}

		for( QString pkPropName: pkPropertyNames)
		{
			EntityDefShd colDef = model.findEntityDef( 
					Model::findByPropertyName{ pkPropName.toLocal8Bit()});
			
			if( colDef )
			{
				pk.push_back( colDef);
				// colDef->isPartOfPrimaryKey = true;
			}
		}

		return pk;
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
	for( auto & fkColDef	: fkDef->relationKey())
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
	const QString classId = QStringLiteral( "class");

	// Table
	m_name = annotation( classId, 
		qe::entity::tags::modelName())
			.value( QString( metaObj->className())).toString();

	// Columns			
	const bool exportParents = annotation( classId,
		  qe::entity::tags::isParentExported())
		.value( false).toBool();
	const int begin = (exportParents) ? 0 : metaObj->propertyOffset();

	for( int i = begin; i < metaObj->propertyCount(); ++i)
	{
		const QMetaProperty property = metaObj->property(i);
		const QByteArray propertyName = property.name();
		const bool isEnable = annotation( propertyName, 
			qe::entity::tags::isEnabled())
				.value( true).toBool();

		if( isEnable )
		{
			EntityDefShd colDef = make_shared<EntityDef>( propertyName, 
					property.type(), *this);
			m_entityDefs.push_back( colDef);
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


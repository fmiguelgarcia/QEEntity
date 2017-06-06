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
#include "ModelPrivate.hpp"
#include "EntityDef.hpp"
#include "RelationDef.hpp"
#include <QMetaProperty>
#include <QStringBuilder>
#include <utility>
#include <memory>

using namespace qe::entity;
using namespace qe::common;
using namespace std;
Q_LOGGING_CATEGORY( qe::entity::lcModel,
	"com.dmious.ipmo.qe.entity.model");

/**
 * \class qe::entity::Model
 * \since 1.0.0
 */

Model::Model( QExplicitlySharedDataPointer<ModelPrivate>&& d)
	: qe::annotation::Model( d)
{}

Model::Model( const QExplicitlySharedDataPointer<ModelPrivate>& d)
	: qe::annotation::Model( d)
{}

Model::Model( const QMetaObject* metaObj)
	: Model(
		QExplicitlySharedDataPointer<ModelPrivate>(
			new ModelPrivate( metaObj)))
{
	Q_D(Model);
	d->parseAnnotations( metaObj);
}

Model::Model(
	const QString & name,
	const EntityDefList& entities /*,
	const Model& refModel*/)
	: qe::annotation::Model(
		QExplicitlySharedDataPointer<ModelPrivate>(
			new ModelPrivate( name, entities)))
{
#if 0
	addReferenceManyToOne(
		QString( "%1_fk_%1").arg( name, refModel.name()).toUtf8(),
		refModel);
#endif
}

bool Model::operator == ( const Model& other) const noexcept
{ return d_ptr == other.d_ptr; }

const QString& Model::name() const noexcept
{
	const Q_D(Model);
	return d->name;
}

const EntityDefList& Model::entityDefs() const noexcept
{
	const Q_D(Model);
	return d->entityDefs;
}

const EntityDefList& Model::primaryKeyDef() const noexcept
{
	const Q_D(Model);
	return d->primaryKey();
}

const RelationDefList & Model::referencesManyToOneDefs() const noexcept
{
	const Q_D(Model);
	return d->referencesManyToOneDefs;
}

void Model::addReferenceManyToOne(
	const QByteArray& propertyName,
	const Model &reference)
{
	Q_D(Model);
	RelationDefShd fkDef = make_shared<RelationDef>( propertyName, reference);

	// Fix fk column names to avoid duplicates.
	for( auto & fkColDef: fkDef->relationKey)
	{
		QString fkColName = fkColDef.entityName();
		auto colDef = findEntityDef( FindEntityDefByEntityName{ fkColName });
		if( colDef )
		{
			uint colissionIdx = 0;
			fkColName = reference.name()
				% QStringLiteral("_") 
				% fkColDef.entityName();

			colDef =  findEntityDef( FindEntityDefByEntityName{ fkColName });
			while( colDef )
			{
				fkColName = QString("%1_%2_%3").arg( reference.name())
					.arg( fkColDef.entityName()).arg( ++colissionIdx);
				colDef = findEntityDef( FindEntityDefByEntityName{ fkColName });
			}
		}
		
		// Update RelationDef 
		fkColDef.setEntityName( fkColName);
		fkColDef.setMappedType( EntityDef::MappedType::ManyToOne);
		d->entityDefs.push_back( fkColDef);
	}

	// Copy ref
	d->referencesManyToOneDefs.push_back( fkDef);
}

optional<EntityDef> Model::findEntityDef(const FindEntityDefByPropertyName& property) const noexcept
{
	const Q_D(Model);
	return d->findEntityDef( property);
}

optional<EntityDef> Model::findEntityDef(const FindEntityDefByEntityName& entity) const noexcept
{
	const Q_D(Model);
	return d->findEntityDef( entity);
}

/// @internal It only searchs on "NoMappedType" fields, in order to find just
/// auto-increment fields in this model.
optional<EntityDef> Model::findEntityDef( const FindEntityDefByAutoIncrement &p) const noexcept
{
	const Q_D(Model);
	return d->findEntityDef( p);
}

optional<EntityDef> Model::findEntityDef(
	EntityDefPredictate&& predicate) const noexcept
{
	const Q_D(Model);
	return d->findEntityDef( std::move(predicate));
}

const RelationDefShd Model::findRelationTo( const Model& model) const noexcept
{
	const Q_D(Model);
	RelationDefShd fk;

	const auto itr = find_if( 
		begin( d->referencesManyToOneDefs),
		end( d->referencesManyToOneDefs),
		[model]( const RelationDefShd& fkDef) -> bool
			{ return fkDef->reference() == model;});

	if( itr != end( d->referencesManyToOneDefs))
		fk = *itr;

	return fk;
}

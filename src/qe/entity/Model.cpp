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
#include "ModelRepository.hpp"
#include "ModelPrivate.hpp"
#include "EntityDef.hpp"
#include "RelationDef.hpp"
#include <qe/common/serialization/QExplicitlySharedDataPointer.hpp>
#include <QMetaProperty>
#include <QStringBuilder>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
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
	const EntityDefList& entities,
	const EntityDefList& primaryKey)
	: qe::annotation::Model(
		QExplicitlySharedDataPointer<ModelPrivate>(
			new ModelPrivate( name, entities, primaryKey)))
{
	ModelRepository::instance().registerModel( *this);
}

bool Model::operator == ( const Model& other) const noexcept
{ return d_ptr == other.d_ptr; }

const EntityDefList& Model::entityDefs() const noexcept
{
	const Q_D(Model);
	return d->entityDefs();
}

const EntityDefList& Model::primaryKeyDef() const noexcept
{
	const Q_D(Model);
	return d->primaryKey();
}

qe::common::optional<RelationDef> Model::referenceManyToOne() const noexcept
{
	const Q_D(Model);
	return d->refManyToOne;
}

void Model::setReferenceManyToOne(
	const QByteArray& propertyName,
	const Model &reference)
{
	Q_D(Model);
	RelationDef fkDef( propertyName, reference);

	// Fix fk column names to avoid duplicates.
	for( auto fkColDef: fkDef.relationKey())
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
		d->pushBackEntityDef( fkColDef);
	}

	// Copy ref
	d->refManyToOne = fkDef;

#if 1
	// Update primary key.
	/// \internal On relations One to many for simple types, the
	/// auto-generated primary key (index) is not unique, and it represents
	/// the current position into the list of elements. This case requires
	/// that foreign key will become part of the key. It also needs to be
	/// the first part in order to optimise searches.
	const EntityDefList& pk = d->primaryKey();
	if ( pk.size() == 1
			&& ! pk.front().isAutoIncrement())
	{
		EntityDefList newPk = fkDef.relationKey();
		newPk.push_back( pk.front());
		d->setPrimaryKey( newPk);
	}
#endif
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

optional<RelationDef> Model::findRelationTo( const Model& model) const noexcept
{
	const Q_D(Model);
	optional<RelationDef> fk;

	if( d->refManyToOne && d->refManyToOne->reference() == model)
		fk = d->refManyToOne;

	return fk;
}

template
void Model::serialize<boost::archive::polymorphic_oarchive>(
	boost::archive::polymorphic_oarchive& oa,
	const unsigned int);

template
void Model::serialize<boost::archive::polymorphic_iarchive>(
	boost::archive::polymorphic_iarchive& ia,
	const unsigned int);


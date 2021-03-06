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
#include "RelationDef.hpp"

#include <qe/common/serialization/QExplicitlySharedDataPointer.hpp>
#include <qe/common/serialization/QByteArray.hpp>
#include <qe/common/serialization/QMetaEnum.hpp>

#include <qe/annotation/ModelPrivate.hpp>
#include <qe/entity/Model.hpp>
#include <qe/entity/EntityDef.hpp>
#include <qe/entity/EntityDefPrivate.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>

using namespace qe::entity;
using namespace std;

RelationDef::RelationDef()
{}

RelationDef::RelationDef( 
		const QByteArray& propertyName,
		const Model& reference )
	: m_propertyName( propertyName), m_reference( reference)
{
	for( const auto& pkDef : reference.primaryKeyDef())
	{
		EntityDef fk = pkDef;
		fk.detach();
		m_relationKey.push_back( fk);
	}
}

RelationDef::RelationDef( const RelationDef& other) noexcept
	: m_propertyName( other.m_propertyName),
	m_reference( other.m_reference),
	m_relationKey( other.m_relationKey)
{}

RelationDef& RelationDef::operator = ( const RelationDef& other) noexcept
{
	m_propertyName = other.m_propertyName;
	m_reference = other.m_reference;
	m_relationKey = other.m_relationKey;

	return *this;
}

RelationDef::~RelationDef()
{}

const QByteArray& RelationDef::propertyName() const noexcept
{ return m_propertyName;}

const Model& RelationDef::reference() const noexcept
{ return m_reference; }

const EntityDefList& RelationDef::relationKey() const noexcept
{ return m_relationKey;}

template< class Archive>
void RelationDef::serialize( Archive& ar, const unsigned int )
{
	// ar & BOOST_SERIALIZATION_NVP( d_ptr);
	ar & boost::serialization::make_nvp( "propertyName", m_propertyName);
	ar & boost::serialization::make_nvp( "reference", m_reference);
	ar & boost::serialization::make_nvp( "relationKey", m_relationKey);
}

template
void RelationDef::serialize<boost::archive::polymorphic_oarchive>(
	boost::archive::polymorphic_oarchive& oa,
	const unsigned int);

template
void RelationDef::serialize<boost::archive::polymorphic_iarchive>(
	boost::archive::polymorphic_iarchive& ia,
	const unsigned int);

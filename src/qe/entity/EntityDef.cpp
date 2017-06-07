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
#include "EntityDefPrivate.hpp"
#include <QMetaEnum>
#include <QStringBuilder>
#include <QMetaProperty>
#include <utility>

using namespace qe;
using namespace qe::entity;
using namespace qe::common;
using namespace std;
Q_LOGGING_CATEGORY( qe::entity::lcEntityDef, "com.dmious.qe.entity.EntityDef")


/**
 * @brief It creates an entity definition
 * @param property Property name.
 * @param type Property type.
 * @param model It will use this model to extract annotations.
 */
/*
EntityDef::EntityDef( const QByteArray &propertyName, const int propertyType,
				const QString& entityName, const uint entityMaxLength)
	: d_ptr( new EntityDefPrivate(
		propertyName,
		propertyType,
		entityName,
		entityMaxLength
{}
*/

EntityDef::EntityDef(
	const QByteArray &property,
	const int type,
	const uint maxLength,
	const optional<Model>& model)
	: d_ptr(
		new EntityDefPrivate( property, type, maxLength, model))
{}

/// @brief It is a constructor for Enum types.
EntityDef::EntityDef(
	const QByteArray &property,
	const QMetaEnum& me,
	const optional<Model>& model)
	: d_ptr( new EntityDefPrivate( property, me, model))
{}

EntityDef::EntityDef( EntityDef&& other) noexcept
	: d_ptr( std::move( other.d_ptr))
{}

EntityDef::EntityDef( const EntityDef& other) noexcept
	: d_ptr( other.d_ptr)
{}

EntityDef::~EntityDef()
{}

EntityDef& EntityDef::operator=( const EntityDef& other) noexcept
{
	d_ptr = other.d_ptr;
	return *this;
}

EntityDef& EntityDef::operator=( EntityDef&& other) noexcept
{
	d_ptr = std::move(other.d_ptr);
	return *this;
}

bool EntityDef::operator==( const EntityDef& other) const noexcept
{
	return d_ptr == other.d_ptr;
}

bool EntityDef::operator<( const EntityDef& other) const noexcept
{
	return d_ptr < other.d_ptr;
}

void EntityDef::detach()
{
	d_ptr.detach();
}

const QString& EntityDef::entityName() const noexcept
{
	const Q_D(EntityDef);
	return d->entityName;
}

void EntityDef::setEntityName( const QString& name) noexcept
{
	Q_D(EntityDef);
	d->entityName = name;
}

const QByteArray& EntityDef::propertyName() const noexcept
{
	const Q_D(EntityDef);
	return d->propertyName;
}

const int EntityDef::propertyType() const noexcept
{
	const Q_D(EntityDef);
	return d->propertyType;
}

const QVariant& EntityDef::defaultValue() const noexcept
{
	const Q_D(EntityDef);
	return d->defaultValue;
}

const EntityDef::MappedType EntityDef::mappedType() const noexcept
{
	const Q_D(EntityDef);
	return static_cast<EntityDef::MappedType>( d->mappedType);
}

void EntityDef::setMappedType( const MappedType mt) noexcept
{
	Q_D(EntityDef);
	d->mappedType = mt;
}

const EntityDef::MappedFetch EntityDef::mappedFetch() const noexcept
{
	const Q_D(EntityDef);
	return static_cast<EntityDef::MappedFetch>( d->mappedFetch);
}

optional<qe::entity::Model> EntityDef::mappedModel() const noexcept
{
	const Q_D(EntityDef);
	return d->mappedModel;
}

uint EntityDef::maxLength() const noexcept
{
	const Q_D(EntityDef);
	return d->maxLength;
}

bool EntityDef::isAutoIncrement() const noexcept
{
	const Q_D(EntityDef);
	return d->isAutoIncrement;
}

void EntityDef::setAutoIncrement( const bool value)
{
	Q_D(EntityDef);
	d->isAutoIncrement = value;
}

bool EntityDef::isNullable() const noexcept
{
	const Q_D(EntityDef);
	return d->isNullable;
}

void EntityDef::setNullable(const bool value)
{
	Q_D(EntityDef);
	d->isNullable = value;
}


/// @brief It checks if this entity definition is an Enum.
bool EntityDef::isEnum() const noexcept
{
	const Q_D(EntityDef);
	return static_cast<bool>( d->metaEnum);
}

/// @return It returns the meta-enum information if it is an Enum
/// entity or null in other case.
/// @see EntityDef#isEnum()
optional<QMetaEnum> EntityDef::enumerator() const noexcept
{
	const Q_D(EntityDef);
	return d->metaEnum;
}

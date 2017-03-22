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
#include <qe/entity/Model.hpp>
#include <qe/entity/EntityDef.hpp>

using namespace qe::entity;
using namespace std;

RelationDef::RelationDef( 
		const QByteArray& propertyName,
		const ModelShd& reference )
	: m_propertyName( propertyName), m_reference( reference)
{
	for( const auto& pkDef : reference->primaryKeyDef())
	{
		const EntityDefShd fk = make_shared<EntityDef>(
				pkDef->propertyName(), pkDef->propertyType(),
				pkDef->entityName(), pkDef->maxLength());

		m_relationKey.push_back( fk);
	}
}
	
const EntityDefList & RelationDef::relationKey() const noexcept
{ return m_relationKey;}

const QByteArray& RelationDef::propertyName() const noexcept
{ return m_propertyName;}
	 
ModelShd RelationDef::reference() const noexcept
{ return m_reference; }


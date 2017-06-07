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
#include "AbstractS11nContext.hpp"
using namespace qe::entity;

/** \class AbstractS11nContext 
 * \since 1.0.0
 * \brief It is the base class for all serialization context.
 */

/// @brief It creates an @c AbstractS11nContext with @p pk values as
///	primary key or unique identifier.
AbstractS11nContext::AbstractS11nContext( const QVariantList & pk)
	: m_primaryKeyValues( pk)
{}

/// @internal d_ptr is not released because is not used yet.
AbstractS11nContext::~AbstractS11nContext()
{}	

/// \brief It returns the primary key values.
const QVariantList& AbstractS11nContext::primaryKey() const noexcept
{ return m_primaryKeyValues; }
		
/// \brief It checks if @p other is already inside the current context.
bool AbstractS11nContext::isObjectInContext( const QObject* other) const noexcept
{
	const auto itr = find( begin( m_context), end( m_context), other);
	return itr != end( m_context);
}

const QObject* AbstractS11nContext::top() const noexcept
{
	if( Q_LIKELY( !m_context.empty()))
		return m_context.back();

	return nullptr;
}

/** \class ScopedS11Context
 * \since 1.0.0
 * \brief It is a helper class to associate object context to an scope.
 *
 * It inserts the object \c obj on top of \c context context in its constructor.
 * And it will removed when this object is destroyed.
 */

ScopedS11Context::ScopedS11Context( 
	QObject* obj, 
	const AbstractS11nContext * const context)
: m_s11nContext( context)
{
	m_s11nContext->m_context.push_back( obj);
}

ScopedS11Context::~ScopedS11Context()
{
	m_s11nContext->m_context.pop_back();
}
	

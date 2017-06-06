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
#include "AbstractSerializer.hpp"
#include <qe/entity/ModelRepository.hpp>
#include <qe/entity/Model.hpp>
#include <qe/common/Exception.hpp>

using namespace qe::entity;
using namespace qe::common;
using namespace std;

namespace {
	/// \brief It checks that @p obj is NOT null and returns its valid metaobject.
	/// It throws an exception if any check fails.
	const QMetaObject* checkAndGetMetaObject( QObject* const obj)
	{
		if( !obj)
			Exception::makeAndThrow( 
					QStringLiteral( "Serializer requires not-null target")); 

		const QMetaObject *metaObject = obj->metaObject();
		if( !metaObject)
			Exception::makeAndThrow( 
					QStringLiteral( "Serializer cannot find meta object")); 

		return metaObject;
	}
}

// Class AbstractSerializer
// ============================================================================

AbstractSerializer::AbstractSerializer()
{}

AbstractSerializer::~AbstractSerializer()
{}	
		
void AbstractSerializer::save( 
	QObject* const source, 
	AbstractS11nContext* const context) const
{
	const QMetaObject* metaObject = checkAndGetMetaObject( source);
	Model lmodel = checkAndGetModel( metaObject);

	save( lmodel, source, context);
}

void AbstractSerializer::load( 
	QObject *const target,
	const AbstractS11nContext* const context) const
{
	const QMetaObject* metaObject = checkAndGetMetaObject( target);
	Model lmodel = checkAndGetModel( metaObject);

	load( lmodel, target, context);
}

Model AbstractSerializer::checkAndGetModel( const QMetaObject* metaObject) const
{
	return ModelRepository::instance().model( metaObject);
}

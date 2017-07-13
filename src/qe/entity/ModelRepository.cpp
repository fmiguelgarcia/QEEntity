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
#include "ModelRepository.hpp"
#include "Model.hpp"
#include "EntityDef.hpp"

using namespace qe::entity;
using namespace std;

namespace {
	
	/// @brief Utility to use double check locking and create/insert objects
	/// into a map if they do NOT exist previously.
	template <typename C, typename K, typename M, typename F>
		typename C::mapped_type findOrCreateUsingDoubleCheckLocking(
				C& container, K& key, M& mutex,
				const F& createFunc)
		{
			auto itr = container.find( key);
			if( itr == std::end( container))
			{
				std::lock_guard<M> _( mutex);
				itr = container.find( key);
				if( itr == std::end( container))
				{
					const auto value = createFunc();
					itr = container.insert( key, value);
				}
			}
			return itr.value();
		}
}

ModelRepository& ModelRepository::instance()
{
	static unique_ptr<ModelRepository> instance;
	static once_flag onceFlag;

	call_once( onceFlag, 
		[]{ instance.reset( new ModelRepository);});

	return *instance.get();
}
			
ModelRepository::ModelRepository() = default;

Model ModelRepository::model( const QMetaObject *metaObject) const
{
	return findOrCreateUsingDoubleCheckLocking( 
		m_modelByMO, metaObject, m_modelsMtx,
		[this,metaObject](){ return makeModel( metaObject);});
}

qe::common::optional<Model> ModelRepository::model(const QString& name) const
{
	qe::common::optional<Model> model;

	const auto itr = m_modelByName.find( name);
	if( itr != end( m_modelByName))
		model = itr.value();

	return model;
}

Model ModelRepository::makeModel( const QMetaObject* metaObj) const
{ 
	Model lmodel( metaObj);
	registerModel( lmodel);
	return lmodel;
}

void ModelRepository::registerModel( const Model& model) const
{
	std::lock_guard<std::recursive_mutex> _( m_modelsMtx);
	m_modelByName.insert( model.name(), model);
}


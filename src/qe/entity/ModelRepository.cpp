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
	template <typename V,  typename K, typename M, typename F>
		V findOrCreateUsingDoubleCheckLocking( 
				std::map<K,V>& container, K& key, M& mutex, 
				const F& createFunc)
		{
			auto itr = container.find( key);
			if( itr == std::end( container))
			{
				std::lock_guard<M> _( mutex);
				itr = container.find( key);
				if( itr == std::end( container))
				{
					V value = createFunc(); 
					itr = container.insert(
							std::make_pair( key, value) ).first;
				}
			}
			return itr->second;
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

ModelShd ModelRepository::model( const QMetaObject *metaObject) const
{
	return findOrCreateUsingDoubleCheckLocking( 
		m_models, metaObject, m_modelsMtx, 
		[this,metaObject](){ return makeModel( metaObject);});
}

ModelShd ModelRepository::makeModel( const QMetaObject* metaObj) const
// { return make_shared<Model>( metaObj);}
{ 
	ModelShd lmodel ( new Model( metaObj));

	// Relation one to many
	for( const auto& eDef: lmodel->entityDefs())
	{
		const auto mappingType = eDef.mappedType();
		if( mappingType == EntityDef::MappedType::OneToMany)
		{
			const auto mo = eDef.mappedModel();
			if( mo )
				mo->addReferenceManyToOne(
					eDef.propertyName(),
					lmodel);
		}
	}

	return lmodel;
}	


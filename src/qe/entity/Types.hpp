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
#pragma once
#include <QString>
#include <memory>
#include <vector>
#include <deque>

class QObject;
namespace qe { namespace entity { 

	class EntityDef;
	class RelationDef;
	class Model;

	using EntityDefShd = std::shared_ptr<EntityDef>;
	using EntityDefList = std::vector<EntityDefShd>;
	using RelationDefShd = std::shared_ptr<RelationDef>;
	using RelationDefList= std::vector<RelationDefShd>;
	using ModelShd = std::shared_ptr<Model>;

	using ObjectContext = std::deque<QObject*>;

	namespace tags {
		QString modelName() noexcept;

		QString entityName() noexcept;
		QString entityMaxLength() noexcept;
		QString isNullable() noexcept;
		QString isAutoIncrementable() noexcept;
		QString isParentExported() noexcept;
		QString isEnabled() noexcept;
	
		QString mappingType() noexcept;
		QString mappingEntity() noexcept;
	
		QString primaryKey() noexcept;
	}

}}

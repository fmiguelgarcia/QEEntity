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
#include <qe/entity/Global.hpp>
#include <qe/common/Common.hpp>
#include <QString>
#include <memory>
#include <vector>
#include <deque>

class QObject;
namespace qe { namespace entity { 

	// Forward declarations
	class EntityDef;
	class RelationDef;
	class Model;

	using EntityDefList = std::vector<EntityDef>;
	using EntityDefPredictate = std::function< bool( const EntityDef& )>;

	using RelationDefShd = std::shared_ptr<RelationDef>;
	using RelationDefList= std::vector<RelationDefShd>;

	struct FindEntityDefByPropertyName { const QByteArray name; };
	struct FindEntityDefByEntityName { const QString name; };
	struct FindEntityDefByAutoIncrement {};

	// Context
	using ObjectContext = std::deque<QObject*>;

	// Tags
	namespace tags {
		QString QEENTITY_EXPORT modelName() noexcept;

		QString QEENTITY_EXPORT entityName() noexcept;
		QString QEENTITY_EXPORT entityMaxLength() noexcept;
		QString QEENTITY_EXPORT isNullable() noexcept;
		QString QEENTITY_EXPORT isAutoIncrementable() noexcept;
		QString QEENTITY_EXPORT isParentExported() noexcept;
		QString QEENTITY_EXPORT isEnabled() noexcept;

		QString QEENTITY_EXPORT mappingType() noexcept;
		QString QEENTITY_EXPORT mappingEntity() noexcept;

		QString QEENTITY_EXPORT primaryKey() noexcept;
	}
}}

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
#include "Types.hpp"

namespace qe { namespace entity { namespace tags {

	QString entityName() noexcept
	{ return QStringLiteral( "@qe.entity.name");}

	QString isNullable() noexcept
	{ return QStringLiteral( "@qe.entity.isNullable");}

	QString isAutoIncrementable() noexcept
	{ return QStringLiteral( "@qe.entity.isAutoIncrementable");}

	QString entityMaxLength() noexcept
	{ return QStringLiteral( "@qe.entity.maxLength");}

	QString mappingType() noexcept
	{ return QStringLiteral( "@qe.entity.mapping.type");}

	QString mappingEntity() noexcept
	{ return QStringLiteral( "@qe.entity.mapping.entity");}

	QString primaryKey() noexcept
	{ return QStringLiteral( "@qe.entity.primaryKey");}

	QString isParentExported() noexcept
	{ return QStringLiteral( "@qe.entity.isParentExported");}

	QString isEnabled() noexcept
	{ return QStringLiteral( "@qe.entity.isEnabled");}

	QString modelName() noexcept
	{ return QStringLiteral( "@qe.model.name");}

#if 0


	QString defaultValue() noexcept
	{ return QStringLiteral( "@qe.orm.default");}

	QString classId() noexcept
	{ return QStringLiteral( "class");}


	QString isTemporaryTable() noexcept
	{ return QStringLiteral( "@qe.orm.isTemporaryTable");}

	QString index() noexcept
	{ return QStringLiteral( "@qe.orm.index");}

#endif
}}}

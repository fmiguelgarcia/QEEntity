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
#include <QLoggingCategory>
#include <unordered_map>

namespace qe { namespace entity {
	Q_DECLARE_LOGGING_CATEGORY( lcAssociativeContainerRegister);

	/// @brief Associative container information.
	struct AssociativeContainerInfo
	{
		const int containerTypeId;		///< Container type ID.
		const int keyTypeId;				///< Key type ID.
		const int valueTypeId;			///< Value type ID.
	};

	class AssociativeContainerRegister
	{
		public:
			using TypeMap = std::unordered_map<int, AssociativeContainerInfo>;
			static AssociativeContainerRegister& instance();

			bool contains( const int containerTypeId) const noexcept;
			AssociativeContainerInfo value( const int containerTypeId) const noexcept;

			int add(
					const int containerTypeId,
					const int keyTypeId,
					const int valueTypeId);

		private:
			AssociativeContainerRegister();
			AssociativeContainerRegister( const AssociativeContainerRegister& ) = delete;

			TypeMap m_types;
	};
}}

#define QE_REGISTER_ASSOCIATIVE_CONTAINER( CONTAINER_TYPE_ID, KEY_TYPE_ID, VALUE_TYPE_ID) \
	namespace { \
		const int QE_ENTITY_UNIQUE_NAME(QE_ASSOC_) = \
			qe::entity::AssociativeContainerRegister::instance() \
				.add( CONTAINER_TYPE_ID, KEY_TYPE_ID, VALUE_TYPE_ID); \
	}

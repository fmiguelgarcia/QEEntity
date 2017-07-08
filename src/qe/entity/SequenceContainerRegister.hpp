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
#include <unordered_map>

namespace qe { namespace entity {

	/// @brief Sequence container information.
	struct SequenceContainerInfo
	{
		const int containerTypeId;		///< Container type ID.
		const int elementTypeId;			///< Element type ID.
	};

	/// @brief It registers information about sequence containers.
	class QEENTITY_EXPORT SequenceContainerRegister
	{
		public:
			using TypeMap = std::unordered_map<int, SequenceContainerInfo>;
			static SequenceContainerRegister& instance();

			bool contains( const int containerTypeId) const noexcept;
			SequenceContainerInfo value( const int containerTypeId) const noexcept;
			int add( const int containerTypeId, const int elementTypeId);

		private:
			SequenceContainerRegister();
			SequenceContainerRegister(
					const SequenceContainerRegister& ) = delete;

			TypeMap m_types;
	};
}}

/// @brief This macro helps to register sequence container information in
/// unit initialization.
/// It creates a unique static int variable, which will store the return value
/// of @c SequenceContainerRegister registration.
#define QE_REGISTER_SEQUENCE_CONTAINER(CONTAINER_TYPE_ID, ELEMENT_TYPE_ID)	\
	namespace { \
		const int QE_ENTITY_UNIQUE_NAME(QE_SEC_) = \
			qe::entity::SequenceContainerRegister::instance() \
				.add( CONTAINER_TYPE_ID, ELEMENT_TYPE_ID); \
	}

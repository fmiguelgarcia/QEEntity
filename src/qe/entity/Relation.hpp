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
#include <qe/entity/ERItem.hpp>
#include <qe/entity/Global.hpp>
#include <qe/entity/Types.hpp>

namespace qe { namespace entity { 
	class RelationPrivate;

	/// @brief Foreign key definition.
	class QEENTITY_EXPORT Relation : public ERItem
	{
		// friend class boost::serialization::access;
		public:
#if 0
			/// @brief Constructor.
			/// @param propertyName Property name.
			/// @param reference Reference entity.
			RelationDef(
					const QByteArray& propertyName,
					const Model& reference);
			RelationDef( const RelationDef& other) noexcept;
			RelationDef& operator = ( const RelationDef& other) noexcept;
			~RelationDef();

			/// @return It returns the property name.
			const QByteArray& propertyName() const noexcept;

			/// @return It return the reference model.
			const Model& reference() const noexcept;

			const EntityDefList& relationKey() const noexcept;


		private:
			/// @note Used only by serialization
			RelationDef();

			QByteArray m_propertyName;
			Model m_reference;
			EntityDefList m_relationKey;
#else
		Relation(
			ERItemShd& source,
			const ERItemShd& target);

#endif
		template< class Archive>
			void serialize( Archive& ar, const unsigned int );

		protected:
			RelationPrivate* d_ptr;

		private:
			ERItemShd m_source;
			ERItemShd m_target;

			Q_DECLARE_PRIVATE( Relation);
	};
}}

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
 * $QE_END_LICENSE$
 *
 */

#pragma once
#include <qe/common/Optional.hpp>
#include <qe/entity/EntityConstraints.hpp>
#include <qe/entity/ERItem.hpp>
#include <QExplicitlySharedDataPointer>
#include <QMetaEnum>

namespace qe { namespace entity {
	class EntityPrivate;
	class EntityConstraints;

	class Entity : public qe::entity::ERItem
	{
		template< class T>
		using optional = qe::common::optional<T>;

		public:
			explicit Entity(
				const QByteArray &property,
				const int type = 0,
				const EntityConstraints& constraints = EntityConstraints());

			Entity(
				const QByteArray &property,
				const QMetaEnum& me);


#if 0
			// Property
			bool isEnum() const noexcept;
			optional<QMetaEnum> enumerator() const noexcept;

			const EntityConstraints& constraints() const;

			// const optional<Relation>& relationDef() const;

			// Other

#endif
			template< class Archive>
			void serialize( Archive & ar, const unsigned int);

		protected:
			// explicit Entity( const QMetaObject* mo);
			// QExplicitlySharedDataPointer<EntityPrivate> d_ptr;

		private:
			Q_DECLARE_PRIVATE( Entity);
	};
}}

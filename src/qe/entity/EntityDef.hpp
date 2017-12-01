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

#pragma once
#include <qe/common/Common.hpp>
#include <qe/entity/Global.hpp>
#include <qe/entity/Model.hpp>
#include <QLoggingCategory>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QMetaEnum>
#include <memory>

namespace qe { namespace entity {
	class Model;
	class EntityDefPrivate;
	class EntityDefConstraints;
	Q_DECLARE_LOGGING_CATEGORY( lcEntityDef);

	/// @brief This class defines the relation between a property and a database
	/// column
    class QEENTITY_EXPORT EntityDef
	{
		friend class boost::serialization::access;
		friend class EntityDefPrivate;
		friend class RelationDef;
		friend class ModelPrivate;
		Q_GADGET
		public:
			enum MappedType {
				NoMappedType = 0,
				OneToOne, OneToMany, 
				ManyToOne, ManyToMany };
#if (QT_VERSION >= QT_VERSION_CHECK( 5, 5, 0))
			Q_ENUM( MappedType );
#endif

			enum MappedFetch { Direct, Lazy };
#if (QT_VERSION >= QT_VERSION_CHECK( 5, 5, 0))
			Q_ENUM( MappedFetch );
#endif

			EntityDef(
				const QByteArray &property = QByteArray(),
				const int type = 0,
				const uint maxLength = 0,
				const qe::common::optional<qe::entity::Model>& model =
					qe::common::optional<qe::entity::Model>());

			EntityDef(
				const QByteArray &property,
				const QMetaEnum& me,
				const qe::common::optional<qe::entity::Model>& model =
					qe::common::optional<qe::entity::Model>());

			EntityDef( EntityDef&& ) noexcept;
			EntityDef( const EntityDef& ) noexcept;

			~EntityDef();

			EntityDef& operator=( const EntityDef& other) noexcept;
			EntityDef& operator=( EntityDef&& other) noexcept;
			bool operator==( const EntityDef& other) const noexcept;
			bool operator<( const EntityDef& other) const noexcept;

			// Entity
			const QString& entityName() const noexcept;
			void setEntityName( const QString& name) noexcept;

			const QVariant& defaultValue() const noexcept;

			// Property
			const QByteArray& propertyName() const noexcept;
			const int propertyType() const noexcept;

			bool isEnum() const noexcept;
			qe::common::optional<QMetaEnum> enumerator() const noexcept;

			// Constraints and relations
#if 0
			const MappedType mappedType() const noexcept;
			void setMappedType( const MappedType mt) noexcept;
			const MappedFetch mappedFetch() const noexcept;
			qe::common::optional<qe::entity::Model> mappedModel() const noexcept;
#else
			const qe::entity::EntityDefConstraints& constraints() const;
			const qe::common::optional<qe::entity::RelationDef>& relationDef() const;
#endif

			void detach();

			template< class Archive>
			void serialize( Archive & ar, const unsigned int);

		protected:
			QExplicitlySharedDataPointer<EntityDefPrivate> d_ptr;

			EntityDefPrivate* shdData() const noexcept;

		private:
			Q_DECLARE_PRIVATE( EntityDef);
	};
}}
Q_DECLARE_TYPEINFO( qe::entity::EntityDef, Q_MOVABLE_TYPE);

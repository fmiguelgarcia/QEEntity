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
#include <qe/annotation/ModelPrivate.hpp>
#include <qe/entity/Types.hpp>

class QMetaProperty;
namespace qe { namespace entity {

	class ERItemPrivate //: public qe::annotation::ModelPrivate
	{
#if 0
		friend class boost::serialization::access;
		public:
			explicit ERItemPrivate(
				const QMetaObject* meta = nullptr);
			ERItemPrivate (
				const QString & name,
				const ERItemList& entities,
				const ERItemList& primaryKey);

			virtual const ERItemList& primaryKey() const noexcept;
			virtual void setPrimaryKey( const ERItemList& pk);

			const ERItemList& entityDefs() const noexcept;
			void setEntityDefs( const ERItemList& eDefs);
			void pushBackEntityDef( const EntityDef& eDef);

			void parseAnnotations( const QMetaObject* metaObj);

			qe::common::optional<EntityDef> findEntityDef(
				EntityDefPredictate&& predicate) const noexcept;

			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByAutoIncrement& ) const noexcept;

			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByPropertyName& ) const noexcept;

			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByEntityName& entity) const noexcept;

			template< class Archive>
			void serialize( Archive& ar, const unsigned int );

		public:
			qe::common::optional<RelationDef> refManyToOne;	///< Many to one defs.

		private:
			void parseAnnotation(
				const QMetaObject* metaObj,
				const QMetaProperty& property);

			ERItemList m_primaryKeyDef;		///< Model primary key definition.
			ERItemList m_entityDefs;			///< Model column definitions.
#endif
	};
}}


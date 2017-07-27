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
#include <qe/entity/EntityDef.hpp>
#include <qe/entity/Model.hpp>
#include <QSharedData>
#include <memory>

namespace qe { namespace entity {
	class Model;
	class EntityDefPrivate : public QSharedData
	{
		friend class boost::serialization::access;
		private:
			/// @note Only used by Serialization
			EntityDefPrivate();

			void decodeProperties(const qe::entity::Model &model);
			void decodeOneToManyAnnotatedRelations( const qe::entity::Model &model);
			void decodePointerRelations( const qe::entity::Model &model);
			void decodeSequentialContainerRelations( const qe::entity::Model &model);
			void decodeAssociativeContainerRelations( const qe::entity::Model &model);

			bool isSequentialContainer() const noexcept;
			bool isAssociativeContainer() const noexcept;

		public:
			EntityDefPrivate(
				const QByteArray& property,
				const int type,
				const uint maxLength,
				const qe::common::optional<qe::entity::Model>& model);

			EntityDefPrivate(
				const QByteArray& property,
				const QMetaEnum metaEnum,
				const qe::common::optional<qe::entity::Model>& model);

			template< class Archive>
			void serialize( Archive & ar, const unsigned int )
			{
				ar & BOOST_SERIALIZATION_NVP( entityName);
				ar & BOOST_SERIALIZATION_NVP( defaultValue);
				ar & BOOST_SERIALIZATION_NVP( propertyName);
				ar & BOOST_SERIALIZATION_NVP( propertyType);
				ar & BOOST_SERIALIZATION_NVP( maxLength);
				ar & BOOST_SERIALIZATION_NVP( isAutoIncrement);
				ar & BOOST_SERIALIZATION_NVP( isNullable);

				ar & BOOST_SERIALIZATION_NVP( mappedType);
				ar & BOOST_SERIALIZATION_NVP( mappedFetch);
				ar & BOOST_SERIALIZATION_NVP( mappedModel);
				ar & BOOST_SERIALIZATION_NVP( metaEnum);
			}

		public:
			QString entityName;					///< Entity name
			QVariant defaultValue;				///< Default value.

			// Property link
			QByteArray propertyName;		///< Property name.
			int propertyType;				///< Property type.

			// Mapped Type
			int mappedType;
			int mappedFetch;
			qe::common::optional<qe::entity::Model> mappedModel;

			// Enum
			qe::common::optional<QMetaEnum> metaEnum;	///< It stores meta-enum info.

			// Constraints
			uint maxLength 			= 0;
			bool isAutoIncrement 	= false;
			bool isNullable 		= true;
	};
}}


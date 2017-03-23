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
#include <QLoggingCategory>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <memory>

namespace qe { namespace annotation { class Model; }}
namespace qe { namespace entity {

	class EntityDefPrivate;
	Q_DECLARE_LOGGING_CATEGORY( lcEntityDef);

	/// @brief This class defines the relation between a property and a database
	/// column
	class EntityDef
	{
		Q_GADGET
		public:
			enum MappingType { 
				NoMappingType, 
				OneToOne, OneToMany, 
				ManyToOne, ManyToMany };
			Q_ENUM( MappingType );

			enum MappingFetch { Direct, Lazy };
			Q_ENUM( MappingFetch );

			/// @brief It creates an entity definition 
			/// @param property Property name.
			/// @param type Property type.
			/// @param model It will use this model to extract annotations.
			EntityDef( const QByteArray &property, const int type, 
					const qe::annotation::Model &model);

			/// @brief  It creates a entity definition 
			/// @param propertyName Property name.
			/// @param propertyType Property type.
			/// @param entityName Entity name.
			/// @param entityMaxLength Entity maximum length constraint. 
			EntityDef( const QByteArray &propertyName, const int propertyType,
					const QString& entityName, const uint entityMaxLength = 0);

			inline const QByteArray& propertyName() const noexcept
			{ return m_propertyName;}

			inline const int propertyType() const noexcept
			{ return m_propertyType;}

			inline const QString& entityName() const noexcept
			{ return m_entityName;}

			void setEntityName( const QString& name) noexcept
			{ m_entityName = name;}

			inline const QVariant& defaultValue() const noexcept
			{ return m_defaultValue;}

			inline const MappingType mappingType() const noexcept
			{ return m_mappingType;}

			void setMappingType( const MappingType mt) noexcept
			{ m_mappingType = mt;}

			inline const MappingFetch mappingFetch() const noexcept
			{ return m_mappingFetch;}
			
			inline const QMetaObject* mappingEntity() const noexcept
			{ return m_mappingEntity;}

			inline uint maxLength() const noexcept
			{ return m_entityMaxLength;}

			inline bool isAutoIncrement() const noexcept
			{ return m_isAutoIncrement;}

			inline bool isNullable() const noexcept
			{ return m_isNullable;}

		private:
			EntityDef( const EntityDef& );

			/// @brief It decodes properties for this entity from @p model.
			void decodeProperties(const qe::annotation::Model &model);

			/// @brief It decodes mapping information.
			void decodeMapping(const qe::annotation::Model &model);

			const QByteArray m_propertyName;		///< Property name.
			const int m_propertyType;				///< Property type.

			QString m_entityName;		///< DB column name
			QVariant m_defaultValue;	///< Default value.

			MappingType m_mappingType 		= MappingType::NoMappingType;
			MappingFetch m_mappingFetch 	= MappingFetch::Direct;
			const QMetaObject* m_mappingEntity = nullptr;

			uint m_entityMaxLength 	= 0;
			bool m_isAutoIncrement 	= false;
			bool m_isNullable	 		= true;
	};
}}

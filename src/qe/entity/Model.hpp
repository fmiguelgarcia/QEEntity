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
#include <qe/common/Common.hpp>
#include <qe/entity/Global.hpp>
#include <qe/entity/Types.hpp>
#include <qe/annotation/Model.hpp>
#include <QLoggingCategory>
#include <QMetaObject>
#include <functional>

namespace qe { namespace entity
{
	class ModelRepository;

	Q_DECLARE_LOGGING_CATEGORY( lcModel);
	/// @brief This model parse the Orm annotations
    class QEENTITY_EXPORT Model : public qe::annotation::Model
	{
		friend class qe::entity::ModelRepository;
		public:
			using FindColDefPredicate = std::function<bool(const EntityDef&)>;

			struct findByPropertyName { const QByteArray name; };
			struct findByEntityName { const QString name; };
			struct findByAutoIncrement {};

			Model(
				const QString & name,
				const EntityDefList& entities);

			// DB 	
			/// @return Database table name.
			const QString& name() const noexcept;

			/// @return Model column definitions.
			const EntityDefList& entityDefs() const noexcept;

			/// @return Model primary key definition.
			const EntityDefList& primaryKeyDef() const noexcept;

			/// @return A list of references of type Many to one.
			const RelationDefList& referencesManyToOneDefs() const noexcept;

			/// @brief Add reference Many (this model) to one @p reference
			void addReferenceManyToOne( const QByteArray& propertyName, 
					const ModelShd& reference);

			// Find utils	
			/// @brief It finds column definitions by property name.
			qe::common::optional<EntityDef> findEntityDef( const findByPropertyName& pn) const noexcept;

			/// @brief It finds column definitions by table column name.
			qe::common::optional<EntityDef> findEntityDef( const findByEntityName& cn) const noexcept;
			
			/// @brief It finds column definition if auto_increment is enabled. 
			qe::common::optional<EntityDef> findEntityDef( const findByAutoIncrement& ) const noexcept;
		
			/// @brief It finds the column definition that returns true on @p
			/// predicate.
			qe::common::optional<EntityDef> findEntityDef( FindColDefPredicate&& predicate) const noexcept;

			/// @brief It returns the foreign key definition for specific model. 
			const RelationDefShd findRelationTo( const ModelShd& model) const noexcept;	

		protected:
			/// @brief Create and parse annotations from @p meta.
			explicit Model( const QMetaObject* meta);

			Model( const Model&) = delete;
			Model& operator=( const Model&) = delete;

		private:
			/// @brief It parses the annotation from @p meta.
			void parseAnnotations( const QMetaObject* meta);

			QString m_name;							///< Model name.
			EntityDefList m_entityDefs;			///< Model column definitions.
			EntityDefList m_primaryKeyDef;		///< Model primary key definition.

			RelationDefList m_referencesManyToOneDefs;	///< Many to one defs.
	};
}}

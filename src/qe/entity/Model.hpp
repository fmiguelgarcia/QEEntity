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
#include <qe/entity/Types.hpp>
#include <qe/common/Common.hpp>
#include <qe/annotation/Model.hpp>
#include <QLoggingCategory>
#include <functional>

namespace qe { namespace entity
{
	class ModelPrivate;
	class ModelRepository;
	Q_DECLARE_LOGGING_CATEGORY( lcModel);

	/// @brief This model parse the Orm annotations
    class QEENTITY_EXPORT Model : public qe::annotation::Model
	{
		friend class qe::entity::ModelRepository;
		public:

			Model(
				const QString & name,
				const EntityDefList& entities /*,
				const Model& refModel*/);

			explicit Model( QExplicitlySharedDataPointer<ModelPrivate>&& d);
			explicit Model( const QExplicitlySharedDataPointer<ModelPrivate>& d);

			bool operator == ( const Model& other) const noexcept;

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
			void addReferenceManyToOne(
				const QByteArray& propertyName,
				const Model& reference);

			/// @brief It finds column definitions by table column name.
			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByEntityName& cn) const noexcept;

			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByAutoIncrement& ) const noexcept;

			/// @brief It finds column definition if auto_increment is enabled.
			qe::common::optional<EntityDef> findEntityDef(
				const FindEntityDefByPropertyName& ) const noexcept;

				/// @brief It finds the column definition that returns true on @p
			/// predicate.
			qe::common::optional<EntityDef> findEntityDef(
				EntityDefPredictate&& predicate) const noexcept;

			/// @brief It returns the foreign key definition for specific model. 
			const RelationDefShd findRelationTo( const Model& model) const noexcept;

		protected:
			/// @brief Create and parse annotations from @p meta.
			explicit Model( const QMetaObject* meta);

		private:
			Q_DECLARE_PRIVATE(Model);
	};
}}

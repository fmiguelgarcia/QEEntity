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
#include <qe/common/Optional.hpp>
#include <qe/entity/Global.hpp>
#include <qe/entity/Types.hpp>
#include <QMetaObject>
#include <QHash>
#include <mutex>

namespace qe { namespace entity {

    class QEENTITY_EXPORT ModelRepository
	{
		friend class Model;
		public:
			static ModelRepository& instance();

			/// @brief It gets the Orm model associated to @p metaObject.
			Model model( const QMetaObject *metaObject) const;

			qe::common::optional<Model>
			model( const QString& name) const;

		private:
			ModelRepository();
			ModelRepository( const ModelRepository&) = delete;
		
			Model makeModel( const QMetaObject* metaObj) const;
			void registerModel( const Model& model) const;

		private:
			using ModelByMetaObject = QHash<const QMetaObject*, Model>;
			using ModelByName = QHash<QString, Model>;

			mutable std::recursive_mutex m_modelsMtx;
			mutable ModelByMetaObject m_modelByMO;
			mutable ModelByName m_modelByName;
	};
}}

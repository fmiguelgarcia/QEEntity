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
#include <qe/entity/Global.hpp>
#include <qe/entity/Types.hpp>
#include <QVariantList>

namespace qe { namespace entity {
	class AbstractS11nContextPrivate;

	/// \internal It is not thread-safe due to \c m_context.
	/// Anyway it should be use inside a thread and NOT share this object
	/// between several ones.
	class QEENTITY_EXPORT AbstractS11nContext
	{
		friend class ScopedS11Context;
		public:
			explicit AbstractS11nContext( const QVariantList & pk);
			virtual ~AbstractS11nContext();

			const QVariantList& primaryKey() const noexcept;
			bool isObjectInContext( const QObject* other) const noexcept;

		protected:
			AbstractS11nContextPrivate* d_ptr;
			mutable ObjectContext m_context;

		private:
			const QVariantList m_primaryKeyValues;

			Q_DECLARE_PRIVATE(AbstractS11nContext);
	};

	class QEENTITY_EXPORT ScopedS11Context
	{
		public:
			ScopedS11Context( 
				QObject* obj, 
				const AbstractS11nContext * const context);
			~ScopedS11Context();

		private:
			const AbstractS11nContext * const m_s11nContext;
	};


}}

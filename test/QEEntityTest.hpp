#pragma once
#include <qe/entity/Types.hpp>
#include <QObject>
#include <memory>

class Book;
class QEEntityTest
	: public QObject
{
	Q_OBJECT
	public:
		QEEntityTest();
		~QEEntityTest();
		
	private slots:
		void checkModelName();
		void checkEntityIsEnable();
		void checkEntityIsParentExported();
		void checkEntityPrimaryKey();
		void checkEntityReferenceOneToMany();
		void checkEntityMappingEntity();
		void checkEntityMappingType();
		void checkEntityMaxLength();
		void checkEntityIsAutoIncrementable();
		void checkEntityIsNullable();
		void checkEntityName();
		
	private:
		std::unique_ptr<Book> m_book;
		qe::entity::ModelShd m_bookModel;
		qe::entity::ModelShd m_chapterModel;
};


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
		void checkEntityOneToManyAutoDetect();
		
	private:
		std::unique_ptr<Book> m_book;
		std::unique_ptr<qe::entity::Model> m_bookModel;
		std::unique_ptr<qe::entity::Model> m_chapterModel;
};


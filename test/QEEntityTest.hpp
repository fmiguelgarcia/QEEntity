#pragma once
#include <QObject>
#include <memory>

class Book;
namespace qe { namespace entity { class Model;}}
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
		void checkEntityMappingEntity();
		void checkEntityMappingType();
		void checkEntityMaxLength();
		void checkEntityIsAutoIncrementable();
		void checkEntityIsNullable();
		void checkEntityName();
		
	private:
		std::unique_ptr<Book> m_book;
		std::unique_ptr<qe::entity::Model> m_bookModel;
		std::unique_ptr<qe::entity::Model> m_chapterModel;
};


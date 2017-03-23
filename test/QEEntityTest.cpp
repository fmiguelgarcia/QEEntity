#include "QEEntityTest.hpp"
#include "entity/book.hpp"

#include <qe/entity/Model.hpp>
#include <qe/entity/EntityDef.hpp>
#include <QTest>
#include <memory>

using namespace qe::entity;
using namespace std;

QEEntityTest::QEEntityTest()
{
	m_book.reset( new Book);
	m_book->chapters.emplace_back( "Chapter 1");
	m_book->chapters.emplace_back( "Chapter 2");

	// Models
	m_bookModel.reset( new Model( m_book->metaObject()));
	m_chapterModel.reset( new Model( m_book->chapters.front().metaObject()));
}

QEEntityTest::~QEEntityTest()
{}

void QEEntityTest::checkModelName()
{
	QVERIFY( m_bookModel->name() == "book");
	QVERIFY( m_chapterModel->name() == "Chapter");
}
		
void QEEntityTest::checkEntityIsEnable()
{
	auto entityDef = m_bookModel->findEntityDef( Model::findByEntityName{ "entityDisable"});
	QVERIFY( entityDef.get() == nullptr);

	entityDef = m_bookModel->findEntityDef( Model::findByEntityName{ "bid"});
  	QVERIFY( entityDef.get() != nullptr );	
}

void QEEntityTest::checkEntityIsParentExported()
{ 
	auto entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "objectName"});
	QVERIFY( entityDef.get() == nullptr);

	entityDef = m_chapterModel->findEntityDef( Model::findByPropertyName{ "objectName"});
	QVERIFY( entityDef.get() != nullptr);
}

void QEEntityTest::checkEntityPrimaryKey()
{ }

void QEEntityTest::checkEntityMappingEntity()
{ 
	auto eDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "chapters"});
	QVERIFY( eDef 
			&& eDef->mappingType() == EntityDef::MappingType::OneToMany 
			&& eDef->mappingEntity()
			&& QStringLiteral("Chapter") == eDef->mappingEntity()->className());

	eDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "title"});
	QVERIFY( eDef 
			&& eDef->mappingType() == EntityDef::MappingType::NoMappingType
			&& eDef->mappingEntity() == nullptr);
}

void QEEntityTest::checkEntityMappingType()
{ }

void QEEntityTest::checkEntityMaxLength()
{ 
	auto entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->maxLength() == 256);

	entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "id"});
	QVERIFY( entityDef && entityDef->maxLength() == 0);
}

void QEEntityTest::checkEntityIsAutoIncrementable()
{
	auto entityDef = m_bookModel->findEntityDef( Model::findByAutoIncrement{});
 	QVERIFY( entityDef && entityDef->isAutoIncrement());

	entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "pages"});
	QVERIFY( entityDef && ! entityDef->isAutoIncrement());

	entityDef = m_chapterModel->findEntityDef( Model::findByAutoIncrement{});
	QVERIFY( entityDef.get() == nullptr);
}

void QEEntityTest::checkEntityIsNullable()
{ 
	auto entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "author"});
	QVERIFY( entityDef && entityDef->isNullable());

	entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->isNullable() == false);
}

void QEEntityTest::checkEntityName()
{ 
	auto entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "id"});
	QVERIFY( entityDef && entityDef->entityName() == "bid");
	
	entityDef = m_bookModel->findEntityDef( Model::findByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->entityName() == "title");
}


QTEST_MAIN( QEEntityTest)

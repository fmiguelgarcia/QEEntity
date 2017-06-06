#include "QEEntityTest.hpp"
#include "entity/book.hpp"

#include <qe/entity/ModelRepository.hpp>
#include <qe/entity/Model.hpp>
#include <qe/entity/EntityDef.hpp>
#include <qe/entity/RelationDef.hpp>
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
	m_bookModel.reset(
		new Model(
			ModelRepository::instance().model( m_book->metaObject())));
	m_chapterModel.reset(
		new Model(
			ModelRepository::instance().model( m_book->chapters.front().metaObject())));
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
	auto entityDef = m_bookModel->findEntityDef(
		FindEntityDefByEntityName{ "entityDisable"});
	QVERIFY( !entityDef);

	entityDef = m_bookModel->findEntityDef(
		FindEntityDefByEntityName{ "bid"});
  	QVERIFY( entityDef);
}

void QEEntityTest::checkEntityIsParentExported()
{ 
	auto entityDef = m_bookModel->findEntityDef(
		FindEntityDefByPropertyName{ "objectName"});
	QVERIFY( !entityDef);

	entityDef = m_chapterModel->findEntityDef(
		FindEntityDefByPropertyName{ "objectName"});
	QVERIFY( entityDef);
}

void QEEntityTest::checkEntityPrimaryKey()
{ }

void QEEntityTest::checkEntityReferenceOneToMany()
{
	auto relations = m_chapterModel->referencesManyToOneDefs();
	QVERIFY( relations.size() == 1);

	for( const auto& relation: relations)
	{
		const EntityDefList& relationKey = relation->relationKey;
	  	for( const auto& eDef: relationKey)
		{
			QVERIFY( eDef.mappedType() == EntityDef::MappedType::ManyToOne);
			
			const auto rDef = m_bookModel->findEntityDef( 
				FindEntityDefByPropertyName{ eDef.propertyName()});

			QVERIFY( rDef);
		}	
	}
}

void QEEntityTest::checkEntityMappingEntity()
{ 
	auto eDef = m_bookModel->findEntityDef(
		FindEntityDefByPropertyName { "chapters"});
	QVERIFY( eDef 
			&& eDef->mappedType() == EntityDef::MappedType::OneToMany
			&& eDef->mappedModel()
			&& QStringLiteral("Chapter") == eDef->mappedModel()->name());

	eDef = m_bookModel->findEntityDef(
		FindEntityDefByPropertyName{ "title"});
	QVERIFY( eDef 
			&& eDef->mappedType() == EntityDef::MappedType::NoMappedType
			&& !eDef->mappedModel());
}

void QEEntityTest::checkEntityMappingType()
{ }

void QEEntityTest::checkEntityMaxLength()
{ 
	auto entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->maxLength() == 256);

	entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "id"});
	QVERIFY( entityDef && entityDef->maxLength() == 0);
}

void QEEntityTest::checkEntityIsAutoIncrementable()
{
	auto entityDef = m_bookModel->findEntityDef( FindEntityDefByAutoIncrement{});
 	QVERIFY( entityDef && entityDef->isAutoIncrement());

	entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "pages"});
	QVERIFY( entityDef && ! entityDef->isAutoIncrement());

	entityDef = m_chapterModel->findEntityDef( FindEntityDefByAutoIncrement{});
	QVERIFY( !entityDef);
}

void QEEntityTest::checkEntityIsNullable()
{ 
	auto entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "author"});
	QVERIFY( entityDef && entityDef->isNullable());

	entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->isNullable() == false);
}

void QEEntityTest::checkEntityName()
{ 
	auto entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "id"});
	QVERIFY( entityDef && entityDef->entityName() == "bid");
	
	entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName{ "title"});
	QVERIFY( entityDef && entityDef->entityName() == "title");
}

void QEEntityTest::checkEntityOneToManyAutoDetect()
{
#ifndef QE_ENTITY_SUPPORT_CONTAINERS
	// QSKIP( "Container support is not built");
#endif
	auto entityDef = m_bookModel->findEntityDef( FindEntityDefByPropertyName {"footNotes"});
	QVERIFY( entityDef && entityDef->entityName() == "footNotes");
	QVERIFY( entityDef->mappedType() == EntityDef::MappedType::OneToMany);
}


QTEST_MAIN( QEEntityTest)

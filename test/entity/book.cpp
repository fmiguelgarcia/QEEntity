#include "book.hpp"
#include <qe/entity/SequenceContainerRegister.hpp>
#include <qe/entity/AssociativeContainerRegister.hpp>

using namespace qe::entity;

namespace {
	const int bookTypeId = qRegisterMetaType<Book>();
	const int chaptersTypeId = qRegisterMetaType< std::vector<Chapter> >();
	const int chapterTypeId = qRegisterMetaType<Chapter>();
	const int bookReferencesExtTypeId = qRegisterMetaType< Book::ReferencesExt>();
	const int x = 0;
}

QE_REGISTER_SEQUENCE_CONTAINER(
	QMetaType::type( "std::vector<Chapter>"),
	QMetaType::type( "Chapter"))

QE_REGISTER_ASSOCIATIVE_CONTAINER(
	QMetaType::type( "Book::ReferencesExt"),
	QMetaType::type( "QString"),
	QMetaType::type( "QString"))

Book::Book( const Book& other)
	: id( other.id),
	title( other.title),
	author( other.author),
	pages( other.pages),
	binSignature ( other.binSignature),
	entityDisable( other.entityDisable),
	chapters( other.chapters),
	references( other.references)
{ }

#include "book.hpp"
#include <qe/entity/SequenceContainerRegister.hpp>
#include <qe/entity/AssociativeContainerRegister.hpp>

using namespace qe::entity;

namespace {
	const int bookTypeId = qRegisterMetaType<Book>();
}

QE_REGISTER_SEQUENCE_CONTAINER(
	std::vector<Chapter>,
	Chapter)

QE_REGISTER_ASSOCIATIVE_CONTAINER(
	Book::ReferencesExt,
	QString,
	QString)

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

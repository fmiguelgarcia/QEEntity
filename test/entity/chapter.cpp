#include "entity/chapter.hpp"

namespace {
	static const int ChapterPtrTypeId = qRegisterMetaType<Chapter*>();
}

Chapter::Chapter( const QString& title) noexcept
	:QObject( nullptr), title( title)
{}

Chapter::Chapter( Chapter&& other) noexcept
	: QObject( nullptr),
	id( std::move( other.id)),
	title( std::move( other.title)),
	text( std::move( other.text))
{}

Chapter::Chapter( const Chapter& other) noexcept
	: QObject( nullptr),
	id( other.id),
	title( other.title),
	text( other.text)
{}

Chapter& Chapter::operator=( const Chapter& other)
{
	id = other.id;
	title = other.title;
	text = other.text;

	return *this;
}

bool Chapter::operator==( const Chapter& other) const noexcept
{
	return id == other.id &&
		title == other.title &&
		text == other.text;
}


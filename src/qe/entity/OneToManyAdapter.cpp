#include "OneToManyAdapter.hpp"

using namespace qe::entity;

OneToManySimpleTypeAdapter::OneToManySimpleTypeAdapter( 
		const QVariant& value,
  		int id)
	: m_id( id), m_value( value)
{}

OneToManySimpleTypeAdapter::OneToManySimpleTypeAdapter( 
	const OneToManySimpleTypeAdapter& other ) noexcept
	: m_id( other.m_id), m_value( other.m_value)
{}

OneToManySimpleTypeAdapter::OneToManySimpleTypeAdapter( 
		OneToManySimpleTypeAdapter&& other ) noexcept
	: m_id( other.m_id), m_value( std::move( other.m_value))
{}

OneToManySimpleTypeAdapter& OneToManySimpleTypeAdapter::operator=(
	const OneToManySimpleTypeAdapter& other)
{
	m_id = other.m_id;
	m_value = other.m_value;
	return *this;
}

bool OneToManySimpleTypeAdapter::operator<(
	const OneToManySimpleTypeAdapter& other) const noexcept
{ return m_id < other.m_id;}

int OneToManySimpleTypeAdapter::id() const noexcept
{ return m_id;}

QVariant OneToManySimpleTypeAdapter::value() const noexcept
{ return m_value;}

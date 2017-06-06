#include "OneToManyAdapter.hpp"

using namespace qe::entity;

OneToManySimpleTypeAdapter::OneToManySimpleTypeAdapter( 
		const QString& value,
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



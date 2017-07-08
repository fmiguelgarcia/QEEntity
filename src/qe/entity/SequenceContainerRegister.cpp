#include "SequenceContainerRegister.hpp"
#include <mutex>
#include <memory>

using namespace qe::entity;
using namespace std;

SequenceContainerRegister::SequenceContainerRegister()
{}

SequenceContainerRegister& SequenceContainerRegister::instance()
{
	static unique_ptr<SequenceContainerRegister> instance;
	static once_flag onceFlag;

	std::call_once( onceFlag,
		[]{ instance.reset( new SequenceContainerRegister);});

	return *instance.get();
}

void SequenceContainerRegister::add(
		const int typeId,
		const int valueTypeId)
{
	m_types.emplace(
		typeId,
		SequenceContainerItem{ typeId, valueTypeId });
}

bool SequenceContainerRegister::contains( const int typeId) const noexcept
{ return m_types.find( typeId) != end(m_types);}

SequenceContainerItem SequenceContainerRegister::value( const int type) const noexcept
{
	const auto itr = m_types.find( type);
	if( itr != end( m_types))
		return itr->second;
	return {0,0};
}

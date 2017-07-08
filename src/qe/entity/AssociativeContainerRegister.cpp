#include "AssociativeContainerRegister.hpp"
#include <mutex>
#include <memory>

using namespace qe::entity;
using namespace std;

AssociativeContainerRegister::AssociativeContainerRegister()
{}

AssociativeContainerRegister & AssociativeContainerRegister::instance()
{
	static unique_ptr<AssociativeContainerRegister> instance;
	static once_flag onceFlag;

	std::call_once( onceFlag,
		[]{ instance.reset( new AssociativeContainerRegister);});

	return *instance.get();
}

void AssociativeContainerRegister::add(
		const int associativeTypeId,
		const int keyTypeId,
		const int valueTypeId)
{
	m_types.emplace(
		associativeTypeId,
		AssociativeTypeItem{
			associativeTypeId,
			keyTypeId,
			valueTypeId
		});
}

bool AssociativeContainerRegister::contains( const int typeId) const noexcept
{ return m_types.find( typeId) != end(m_types);}

AssociativeTypeItem AssociativeContainerRegister::value( const int type) const noexcept
{
	const auto itr = m_types.find( type);
	if( itr != end( m_types))
		return itr->second;
	return {0,0,0};
}


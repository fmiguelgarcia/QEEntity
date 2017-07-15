#include "SequenceContainerRegister.hpp"
#include <mutex>
#include <memory>

using namespace qe::entity;
using namespace std;

/// @class qe::entity::SequenceContainerRegister
///
/// This library also register common QT sequence containers:
/// 	- QStringList
/// 	- QByteArrayList
/// 	- QJsonArray
/// 	- QVariantList
/// 	.

QE_REGISTER_SEQUENCE_CONTAINER_ID( QMetaType::QStringList, QMetaType::QString)
QE_REGISTER_SEQUENCE_CONTAINER_ID( QMetaType::QByteArrayList, QMetaType::QByteArray)
QE_REGISTER_SEQUENCE_CONTAINER_ID( QMetaType::QJsonArray, QMetaType::QJsonValue)
QE_REGISTER_SEQUENCE_CONTAINER_ID( QMetaType::QVariantList, QMetaType::QVariant)

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

int SequenceContainerRegister::add(
		const int containerTypeId,
		const int elementTypeId)
{
	m_types.emplace(
		containerTypeId,
		SequenceContainerInfo{ containerTypeId, elementTypeId });

	return containerTypeId;
}

bool SequenceContainerRegister::contains(const int containerTypeId) const noexcept
{ return m_types.find( containerTypeId) != end(m_types);}

SequenceContainerInfo SequenceContainerRegister::value(const int containerTypeId) const noexcept
{
	const auto itr = m_types.find( containerTypeId);
	if( Q_LIKELY(itr != end( m_types)))
		return itr->second;

	return {0,0};
}

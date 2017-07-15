#include "AssociativeContainerRegister.hpp"
#include <mutex>
#include <memory>

using namespace qe::entity;
using namespace std;

QE_REGISTER_ASSOCIATIVE_CONTAINER_ID( QMetaType::QVariantMap, QMetaType::QString, QMetaType::QVariant)
QE_REGISTER_ASSOCIATIVE_CONTAINER_ID( QMetaType::QVariantHash, QMetaType::QString, QMetaType::QVariant)
QE_REGISTER_ASSOCIATIVE_CONTAINER_ID( QMetaType::QJsonObject, QMetaType::QString, QMetaType::QJsonValue)

Q_LOGGING_CATEGORY(
	qe::entity::lcAssociativeContainerRegister,
	"com.dmious.qe.entity.associativeContainerRegister");

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

int AssociativeContainerRegister::add(
		const int containerTypeId,
		const int keyTypeId,
		const int valueTypeId)
{
	qCDebug( lcAssociativeContainerRegister,
			"Add associative container: %d key: %d value: %d",
			containerTypeId, keyTypeId, valueTypeId);

	m_types.emplace(
		containerTypeId,
		AssociativeContainerInfo{
			containerTypeId,
			keyTypeId,
			valueTypeId
		});

	return containerTypeId;
}

bool AssociativeContainerRegister::contains(
		const int containerTypeId) const noexcept
{ return m_types.find( containerTypeId) != end(m_types);}

AssociativeContainerInfo AssociativeContainerRegister::value(
		const int containerTypeId) const noexcept
{
	const auto itr = m_types.find( containerTypeId);
	if( Q_LIKELY( itr != end( m_types)))
		return itr->second;

	return {0,0,0};
}

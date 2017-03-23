#include <QObject>
#include <qe/entity/Model.hpp>
#include <qe/entity/serialization/AbstractSerializedItem.hpp>

using namespace qe::entity;

int main(int argc, char** argv)
{
	QObject * o = new QObject;
	Model annModel( o->metaObject());

	AbstractSerializedItem si;
	Q_UNUSED( si);
	
	return 0;
}

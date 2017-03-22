#include <QObject>
#include <qe/entity/Model.hpp>
using namespace qe::entity;

int main(int argc, char** argv)
{
	QObject * o = new QObject;
	Model annModel( o->metaObject());
	
	return 0;
}

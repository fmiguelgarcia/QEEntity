#include <QObject>
#include <qe/entity/ModelRepository.hpp>

using namespace qe::entity;

int main(int argc, char** argv)
{
	const auto& mrepo = ModelRepository::instance();
	return 0;
}

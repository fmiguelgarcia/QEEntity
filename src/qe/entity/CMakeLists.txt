# Requirements 
include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)
set( QEEntity_MAYOR_VERSION 1)
set( QEEntity_MINOR_VERSION 0)
set( QEEntity_BUILD_VERSION 0)

# Source
set( QEEntity_SRCS
	Global.hpp
	OneToManyAdapter.cpp
	AbstractS11nContext.cpp
	AbstractSerializer.cpp
	Types.cpp
	SequenceContainerRegister.cpp
	AssociativeContainerRegister.cpp
	EntityDefPrivate.cpp
	EntityDef.cpp
	RelationDef.cpp
	ModelPrivate.cpp
	Model.cpp
	ModelRepository.cpp)
add_definitions( -DQEENTITY_LIBRARY)

# Link
add_library( QEEntity SHARED ${QEEntity_SRCS})
target_link_libraries( QEEntity
	Qt5::Core ${CONAN_LIBS}) 
set_target_properties( QEEntity PROPERTIES 
	VERSION ${QEEntity_MAYOR_VERSION}.${QEEntity_MINOR_VERSION}.${QEEntity_BUILD_VERSION}
	SOVERSION ${QEEntity_MAYOR_VERSION})
target_compile_features( QEEntity 
	PUBLIC cxx_auto_type cxx_nullptr cxx_lambdas cxx_uniform_initialization)

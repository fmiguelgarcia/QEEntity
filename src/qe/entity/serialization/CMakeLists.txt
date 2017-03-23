# Requirements 
include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)
set( QEEntityS11n_MAYOR_VERSION 0)
set( QEEntityS11n_MINOR_VERSION 1)
set( QEEntityS11n_BUILD_VERSION 0)

# Source
set( QEEntityS11n_SRCS 
	AbstractSerializedItem.cpp AbstractSerializer.cpp)

# Link
add_library( QEEntityS11n SHARED ${QEEntityS11n_SRCS})
set_target_properties( QEEntityS11n PROPERTIES 
	VERSION
	${QEEntityS11n_MAYOR_VERSION}.${QEEntityS11n_MINOR_VERSION}.${QEEntityS11n_BUILD_VERSION}
	SOVERSION ${QEEntityS11n_MAYOR_VERSION})
target_link_libraries( QEEntityS11n
	Qt5::Core ${CONAN_LIBS}) 
target_compile_features( QEEntityS11n PUBLIC cxx_auto_type cxx_nullptr 
	cxx_lambdas)

# Requirements 
include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)

# Source
set( QEEntityS11n_SRCS 
	AbstractSerializedItem.cpp AbstractSerializer.cpp)

# Link
add_library( QEEntityS11n STATIC ${QEEntityS11n_SRCS})
target_link_libraries( QEEntityS11n
	Qt5::Core ${CONAN_LIBS}) 
target_compile_features( QEEntityS11n PUBLIC cxx_auto_type cxx_nullptr 
	cxx_lambdas)

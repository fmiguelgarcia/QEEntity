include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)
find_package( Qt5Test)
find_package( Boost REQUIRED COMPONENTS serialization )

# Source
set( QEEntityTest_SRCS 
	QEEntityTest.cpp
	entity/chapter.cpp entity/book.cpp)

# Create exec
add_executable( QEEntityTest ${QEEntityTest_SRCS})
target_link_libraries( QEEntityTest 
	QEEntity 
	Qt5::Core Qt5::Test
	Boost::serialization
	${CONAN_LIBS})
target_compile_features( QEEntityTest 
	PUBLIC cxx_auto_type cxx_nullptr cxx_lambdas cxx_uniform_initialization)

# Add test
add_test( NAME QEEntityTest  COMMAND $<TARGET_FILE:QEEntityTest>)

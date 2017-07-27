include(${CMAKE_SOURCE_DIR}/tools/cmake/common.cmake)
find_package( Qt5Test)
find_package( Boost REQUIRED COMPONENTS serialization )

# Source
set( QEEntityTest_SRCS QEEntityTest.cpp
	entity/chapter.cpp entity/book.cpp)

# Create exec
add_executable( QEEntityTest ${QEEntityTest_SRCS})
target_link_libraries( QEEntityTest 
	QEEntity 
	Qt5::Core Qt5::Test
	Boost::serialization
	${CONAN_LIBS})

# Add test
add_test( NAME QEEntityTest  COMMAND QEEntityTest)

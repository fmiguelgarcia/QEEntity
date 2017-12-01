cmake_minimum_required ( VERSION 3.2)
project (QEEntity)
set( QEENTITY_TEST ON CACHE BOOL "Enable unit tests")


add_subdirectory( src/qe/entity)

#if (QEENTITY_TEST)
#	enable_testing() 
#	add_subdirectory( test)
#endif()

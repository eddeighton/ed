cmake_minimum_required(VERSION 2.8)

#we assume workspace directory is set

set( ED_DIR ${WORKSPACE_DIR}/ed )

set( ED_TEST_DIR ${ED_DIR}/tests )
file( GLOB ED_TEST_SOURCES ${ED_TEST_DIR}/*.cpp )
foreach( F ${ED_TEST_SOURCES})
    message( STATUS "Found unit test: " ${F} )
endforeach( F )
source_group( ed/tests FILES ${ED_TEST_SOURCES} )

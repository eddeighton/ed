cmake_minimum_required( VERSION 3.1...3.16 )

project( Ed VERSION 1.0 LANGUAGES CXX )

include( ${WORKSPACE_ROOT_PATH}/src/env/src/common_settings.cmake )

#setup some common directories from the root of the project
set( CMAKE_PARENT_PATH ${CMAKE_CURRENT_LIST_DIR}/.. )
find_path( ED_ROOT_DIR NAMES api PATHS ${CMAKE_PARENT_PATH} )
set( ED_API_DIR ${ED_ROOT_DIR}/api )
set( ED_SRC_DIR ${ED_ROOT_DIR}/src )

include_directories( ${ED_API_DIR} )
include_directories( ${ED_SRC_DIR} )

add_subdirectory( ed )
add_subdirectory( ed_tests )
add_subdirectory( ed_cmdline )

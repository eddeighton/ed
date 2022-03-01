cmake_minimum_required(VERSION 2.8)

#get boost
include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

#get ed
include( ${ED_ROOT_DIR}/cmake/ed_include.cmake )

add_executable( ed
					${ED_ROOT_DIR}/src/main.cpp
				)

set_target_properties( ed PROPERTIES FOLDER ed )
set_target_properties( ed PROPERTIES DEBUG_POSTFIX d )

target_link_libraries( ed edlib )

link_boost( ed filesystem )
link_boost( ed iostreams )
link_boost( ed serialization )
link_boost( ed program_options )
link_common( ed )

install( TARGETS ed DESTINATION bin)

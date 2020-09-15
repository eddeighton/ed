cmake_minimum_required(VERSION 2.8)

#set( POSSIBLE_COMMON_INSTALL ${ED_ROOT_DIR}/../../common/install )
#find_path( COMMON_INSTALL_PATH NAMES /share/common-config.cmake PATHS ${POSSIBLE_COMMON_INSTALL} NO_DEFAULT_PATH  )

#find path is not working...
set( COMMON_INSTALL_PATH ${ED_ROOT_DIR}/../../common/install/ )
message( "Warning - using temporary hack to auto discover common installation" )



set( CMAKE_PREFIX_PATH "${COMMON_INSTALL_PATH}/share;${CMAKE_PREFIX_PATH}" )
find_package( common )

#message( "Ed Root Dir : ${ED_ROOT_DIR}" )
#message( "Found command install at ${ED_ROOT_DIR}/../../common/install" )
#message( "Found command install at ${COMMON_INSTALL_PATH}" )

#include( ${COMMON_INSTALL_PATH}/share/common-config.cmake )

#find_package( common )

function( link_common targetname )
	target_link_libraries( ${targetname} PUBLIC Common::commonlib )
endfunction( link_common )

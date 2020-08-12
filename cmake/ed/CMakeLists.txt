cmake_minimum_required(VERSION 2.8)

#get boost
include( ${ED_ROOT_DIR}/cmake/boost_include.cmake )

#get common
include( ${ED_ROOT_DIR}/cmake/common_include.cmake )

set( ED_SOURCES_API
    ${ED_API_DIR}/ed/argument.hpp
    ${ED_API_DIR}/ed/code.hpp
    ${ED_API_DIR}/ed/declarator.hpp
    ${ED_API_DIR}/ed/ed.hpp
    ${ED_API_DIR}/ed/expression.hpp
    ${ED_API_DIR}/ed/file.hpp
    ${ED_API_DIR}/ed/fileRef.hpp
    ${ED_API_DIR}/ed/identifier.hpp
    ${ED_API_DIR}/ed/node.hpp
    ${ED_API_DIR}/ed/nodeio.hpp
    ${ED_API_DIR}/ed/override.hpp
    ${ED_API_DIR}/ed/parser.hpp
    ${ED_API_DIR}/ed/ref.hpp
    ${ED_API_DIR}/ed/serialise.hpp
    ${ED_API_DIR}/ed/shorthand.hpp
    ${ED_API_DIR}/ed/shorthandio.hpp
    ${ED_API_DIR}/ed/skip.hpp
    ${ED_API_DIR}/ed/statement.hpp
    ${ED_API_DIR}/ed/stlio.hpp
    ${ED_API_DIR}/ed/tag.hpp
    ${ED_API_DIR}/ed/type.hpp )

set( ED_SOURCES_SRC
    ${ED_SRC_DIR}/argument.cpp
    ${ED_SRC_DIR}/code.cpp
    ${ED_SRC_DIR}/declarator.cpp
    ${ED_SRC_DIR}/expression.cpp
    ${ED_SRC_DIR}/file.cpp
    ${ED_SRC_DIR}/fileRef.cpp
    ${ED_SRC_DIR}/identifier.cpp
    ${ED_SRC_DIR}/node.cpp
    ${ED_SRC_DIR}/nodeio.cpp
    ${ED_SRC_DIR}/override.cpp
    ${ED_SRC_DIR}/ref.cpp
    ${ED_SRC_DIR}/serialise.cpp
    ${ED_SRC_DIR}/shorthand.cpp
    ${ED_SRC_DIR}/skip.cpp
    ${ED_SRC_DIR}/statement.cpp
    ${ED_SRC_DIR}/tag.cpp
    ${ED_SRC_DIR}/type.cpp )

set( ED_SOURCES ${ED_SOURCES_API} ${ED_SOURCES_SRC} )

source_group( src FILES ${ED_SOURCES} )

add_library( edlib STATIC 
		${COMMON_INCLUDE_HEADERS}
		${ED_SOURCES} 
		)
		
set_target_properties( edlib PROPERTIES FOLDER edlib )
set_target_properties( edlib PROPERTIES DEBUG_POSTFIX d )

link_boost( edlib filesystem )
link_boost( edlib iostreams )
link_boost( edlib serialization )
link_common( edlib )

install( FILES ${ED_SOURCES_API} DESTINATION include/ed )
install( TARGETS edlib EXPORT ed DESTINATION lib)
install( EXPORT ed NAMESPACE ED:: FILE ed-config.cmake DESTINATION share )
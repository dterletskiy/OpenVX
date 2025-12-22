cmake_minimum_required( VERSION 3.5 )
project( OpenVXExample CXX )

# If OPENVX_ROOT defined cmake, use it, or set it empty
if( NOT DEFINED OPENVX_ROOT )
   set( OPENVX_ROOT "" )
endif()

# If OPENVX_ROOT was not defined, try to find system OpenVX
if( OPENVX_ROOT STREQUAL "" )
   find_path( OPENVX_INCLUDE_DIR NAMES vx.h PATHS /usr/include /usr/local/include )
   find_library( OPENVX_LIBRARY NAMES openvx PATHS /usr/lib /usr/local/lib )
   if( NOT OPENVX_INCLUDE_DIR OR NOT OPENVX_LIBRARY )
      message( FATAL_ERROR
         "OpenVX not found. Please set OPENVX_ROOT or install OpenVX system-wide."
      )
   endif( )
else( )
   set( OPENVX_INCLUDE_DIR "${OPENVX_ROOT}/include" )
   set( OPENVX_LIBRARY "${OPENVX_ROOT}/bin/libopenvx.so" )
   # set( OPENVX_LIBRARY "${OPENVX_ROOT}/bin/libopenvx.a" )
endif( )

add_executable( example
   source/ImageSize.cpp
   source/pgm.cpp
   source/main.cpp
)

target_include_directories( example PRIVATE
   ${OPENVX_INCLUDE_DIR}
)

# target_link_directories( example PRIVATE
#    $<TARGET_FILE_DIR:${OPENVX_LIBRARY}>
# )

target_link_libraries( example
   ${OPENVX_LIBRARY}
)

message( STATUS "Using OpenVX include: ${OPENVX_INCLUDE_DIR}" )
message( STATUS "Using OpenVX library: ${OPENVX_LIBRARY}" )

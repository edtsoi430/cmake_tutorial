
# CMake find script for opengl
# Author: Adam Dziedzic


find_package(PkgConfig)

find_path(EPOXY_INCLUDE_DIR epoxy
	HINTS /usr/local/include /usr/include)

find_library(EPOXY_LIBRARY libepoxy.so
	HINTS /usr/lib /usr/local/lib
	PATH_SUFFIXES x86_64-linux-gnu)

set(EPOXY_LIBRARIES ${EPOXY_LIBRARY})

set(EPOXY_INCLUDE_DIRS ${EPOXY_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Epoxy DEFAULT_MSG
	EPOXY_LIBRARY EPOXY_INCLUDE_DIR)

mark_as_advanced(EPOXY_LIBRARY EPOXY_INCLUDE_DIR)

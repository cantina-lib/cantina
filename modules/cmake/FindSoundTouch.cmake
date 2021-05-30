# - Find the SoundTouch library
#
# Based on FindFFTW.cmake:
#   Copyright (c) 2015, Wenzel Jakob
#   https://github.com/wjakob/layerlab/blob/master/cmake/FindFFTW.cmake, commit 4d58bfdc28891b4f9373dfe46239dda5a0b561c6
#   Copyright (c) 2017, Patrick Bos
#   https://github.com/egpbos/findFFTW/blob/master/FindFFTW.cmake
#
# Usage:
#   find_package(SoundTouch [REQUIRED] [QUIET])
#
# It sets the following variables:
#   SoundTouch_FOUND                  ... true if soundtouch is found on the system
#   SoundTouch_LIBRARIES              ... full paths to all found soundtouch libraries
#   SoundTouch_INCLUDE_DIRS           ... soundtouch include directory paths
#
# The following variables will be checked by the function
#   SoundTouch_USE_STATIC_LIBS        ... if true, only static libraries are found, otherwise both static and shared.
#   SoundTouch_ROOT                   ... if set, the libraries are exclusively searched
#                                   under this path
#
if (NOT SoundTouch_ROOT AND DEFINED ENV{SoundTouchDIR})
    set(SoundTouch_ROOT $ENV{SoundTouchDIR})
endif ()

# Check if we can use PkgConfig
find_package(PkgConfig)

#Determine from PKG
if (PKG_CONFIG_FOUND AND NOT SoundTouch_ROOT)
    pkg_check_modules(PKG_SoundTouch QUIET "SoundTouch")
endif ()

#Check whether to search static or dynamic libs
set(CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES})

if (${SoundTouch_USE_STATIC_LIBS})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
else ()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV})
endif ()

if (SoundTouch_ROOT)
    # find libs
    find_library(
            SoundTouch_LIBRARIES
            NAMES "SoundTouch" "Soundtouch" "soundtouch"
            PATHS ${SoundTouch_ROOT}
            PATH_SUFFIXES "lib" "lib64"
            NO_DEFAULT_PATH
    )

    #find includes
    find_path(SoundTouch_INCLUDE_DIRS
            NAMES "SoundTouch.h"
            PATHS ${SoundTouch_ROOT}
            PATH_SUFFIXES "include" "soundtouch"
            NO_DEFAULT_PATH
            )

else ()

    find_library(
            SoundTouch_LIBRARIES
            NAMES "SoundTouch" "Soundtouch" "soundtouch"
            PATH_SUFFIXES "lib" "lib64"
            PATHS ${PKG_SoundTouch_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
    )

    find_path(SoundTouch_INCLUDE_DIRS
            NAMES "SoundTouch.h"
            PATH_SUFFIXES "include" "soundtouch"
            PATHS ${PKG_SoundTouch_INCLUDE_DIRS} ${INCLUDE_INSTALL_DIR}
            )

endif (SoundTouch_ROOT)

add_library(SoundTouch INTERFACE IMPORTED)
set_target_properties(SoundTouch
        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SoundTouch_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${SoundTouch_LIBRARIES}"
        )

set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SoundTouch
        REQUIRED_VARS SoundTouch_INCLUDE_DIRS
        HANDLE_COMPONENTS
        )

mark_as_advanced(
        SoundTouch_INCLUDE_DIRS
        SoundTouch_LIBRARIES
)

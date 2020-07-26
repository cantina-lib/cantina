cmake_minimum_required(VERSION 3.6)

# to get the parent's project dir, before declaring new project
set(CANTINA_DIR ${PROJECT_SOURCE_DIR})

project(helmholtz)

set(HELMHOLTZ_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(HELMHOLTZ_SRC_DIR ${PROJECT_SOURCE_DIR}/source)

# need cantina_common for types and pd-related stuff

set(PANTOUFLE_DIR ${CANTINA_DIR}/pantoufle)
set(CANTINA_COMMON_DIR ${PANTOUFLE_DIR}/cantina_common)

message(STATUS ${CANTINA_COMMON_DIR})

include_directories(
        ${HELMHOLTZ_INCLUDE_DIR}
        ${CANTINA_COMMON_DIR}/include
        ${CANTINA_DIR}/include
        ${FFTW3_DIR}/api
        )

# pd stuff

set(HELMHOLTZ_LIB
        ${HELMHOLTZ_SRC_DIR}/Helmholtz.cpp
        ${HELMHOLTZ_INCLUDE_DIR}/Helmholtz.h
        )

add_library(helmholtz ${HELMHOLTZ_LIB})

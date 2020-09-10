cmake_minimum_required(VERSION 3.6)

# changing directories.
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(FFTW_DIR ${THIRDPARTY_DIR}/fftw-3.3.8)

# not working currently, will have to ./configure by hand WITH -fPIC !!
# ./configure --enable-float CFLAGS="-fPIC"
option(ENABLE_FLOAT      "Enable 32-bit float version of FFTW3 (fftw3f)" ON)
option(ENABLE_THREADS    "Enable multithreading part of FFTW3"           ON)
option(BUILD_SHARED_LIBS "Build shared library version of FFTW3"         ON)
option(BUILD_TESTS       "Build test benchmarks for FFTW3"               ON)
set(ENABLE_FLOAT ON)
set(ENABLE_THREADS OFF) # for now.
set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
# now adding the generated cmake
cmake_policy(SET CMP0074 OLD)
# cmake_policy(SET CMP0075 OLD)
# add_subdirectory(${FFTW3_DIR})

execute_process(
        COMMAND ./configure --enable-float
        COMMAND make CFLAGS="-fPIC"
        WORKING_DIRECTORY ${FFTW_DIR}
        OUTPUT_QUIET
)

# Setting vars recognised by FindFFTW3
set(FFTW_ROOT ${FFTW_DIR})
set(FFTW_USE_STATIC_LIBS ON)
# find it!
find_package(FFTW REQUIRED COMPONENTS FLOAT_LIB)
set(FFTW3_INCLUDE_DIRS ${FFTW_INCLUDE_DIRS} PARENT_SCOPE)
set(FFTW3_LIBRARIES ${FFTW_LIBRARIES} PARENT_SCOPE)

message(STATUS ${FFTW_LIBRARIES})

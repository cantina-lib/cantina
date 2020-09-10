cmake_minimum_required(VERSION 3.6)

# changing directories.
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(SOUNDTOUCH_DIR ${THIRDPARTY_DIR}/soundtouch)

set(SOUNDTOUCH_SOURCE_DIR   ${SOUNDTOUCH_DIR}/source/SoundTouch)
set(SOUNDTOUCH_INCLUDE_DIR  ${SOUNDTOUCH_DIR}/include)
set(SOUNDTOUCH_INCLUDE_DIRS "${SOUNDTOUCH_INCLUDE_DIR}" "${SOUNDTOUCH_SOURCE_DIR}" PARENT_SCOPE)

project(soundtouch)
# FLAGS FOR LIBS
set(CMAKE_CXX_FLAGS ${CANTINA_THIRDPARTY_CXX_FLAGS})

#
# in order to avoid clicks when the target pitch is close to 1
#
add_compile_definitions(SOUNDTOUCH_PREVENT_CLICK_AT_RATE_CROSSOVER)

include_directories(${SOUNDTOUCH_INCLUDE_DIR})


set(SOUNDTOUCH_LIB
        #source
        ${SOUNDTOUCH_SOURCE_DIR}/SoundTouch.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/AAFilter.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/BPMDetect.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/cpu_detect_x86.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/FIFOSampleBuffer.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/FIRFilter.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateCubic.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateLinear.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateShannon.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/mmx_optimized.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/PeakFinder.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/RateTransposer.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/sse_optimized.cpp
        ${SOUNDTOUCH_SOURCE_DIR}/TDStretch.cpp
        # headers
        ${SOUNDTOUCH_INCLUDE_DIR}/BPMDetect.h
        ${SOUNDTOUCH_INCLUDE_DIR}/FIFOSampleBuffer.h
        ${SOUNDTOUCH_INCLUDE_DIR}/FIFOSamplePipe.h
        ${SOUNDTOUCH_INCLUDE_DIR}/SoundTouch.h
        ${SOUNDTOUCH_INCLUDE_DIR}/STTypes.h
        ${SOUNDTOUCH_INCLUDE_DIR}/soundtouch_config.h
        ${SOUNDTOUCH_SOURCE_DIR}/AAFilter.h
        ${SOUNDTOUCH_SOURCE_DIR}/cpu_detect.h
        ${SOUNDTOUCH_SOURCE_DIR}/FIRFilter.h
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateCubic.h
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateLinear.h
        ${SOUNDTOUCH_SOURCE_DIR}/InterpolateShannon.h
        ${SOUNDTOUCH_SOURCE_DIR}/PeakFinder.h
        ${SOUNDTOUCH_SOURCE_DIR}/RateTransposer.h
        ${SOUNDTOUCH_SOURCE_DIR}/TDStretch.h
        )

add_library(soundtouch STATIC ${SOUNDTOUCH_LIB})
set(SOUNDTOUCH_LIBRARIES soundtouch PARENT_SCOPE)

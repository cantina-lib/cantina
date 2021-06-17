cmake_minimum_required(VERSION 3.6)


# changing directories.
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(SOUNDTOUCH_DIR ${THIRDPARTY_DIR}/soundtouch)

project(soundtouch)

set(SOUNDTOUCH_SOURCE_DIR   ${SOUNDTOUCH_DIR}/source/SoundTouch)
set(SOUNDTOUCH_INCLUDE_DIR  ${SOUNDTOUCH_DIR}/include)

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

add_library(${PROJECT_NAME} STATIC ${SOUNDTOUCH_LIB})

# in order to avoid clicks when the target pitch is close to 1
#
target_compile_definitions(${PROJECT_NAME} PRIVATE SOUNDTOUCH_PREVENT_CLICK_AT_RATE_CROSSOVER)
target_compile_options(${PROJECT_NAME} PRIVATE "-fPIC")

target_include_directories(${PROJECT_NAME} PUBLIC ${SOUNDTOUCH_INCLUDE_DIR})


if (NOT DEFINED SOUNDTOUCH_INIT)
    execute_process(COMMAND
            ./bootstrap
            WORKING_DIRECTORY ${SOUNDTOUCH_DIR}
            )
    execute_process(COMMAND
            ./configure
            WORKING_DIRECTORY ${SOUNDTOUCH_DIR}
            )
    set(SOUNDTOUCH_INIT CACHE INTERNAL "SoundTouch has been initialised.")
endif(NOT DEFINED SOUNDTOUCH_INIT)

set(SOUNDTOUCH_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)

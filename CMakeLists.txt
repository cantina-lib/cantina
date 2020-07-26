cmake_minimum_required(VERSION 3.6)

project(cantina)

set(CANTINA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(CANTINA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(CANTINA_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(CANTINA_THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)

set(CMAKE_CXX_STANDARD 20)

# FLAGS FOR LIBS
set(CMAKE_CXX_FLAGS "-fPIC")

#fftw
set(FFTW3_DIR ${CANTINA_THIRDPARTY_DIR}/fftw-3.3.8)
add_subdirectory(${FFTW3_DIR})


# soundtouch
set(SOUNDTOUCH_DIR ${CANTINA_THIRDPARTY_DIR}/soundtouch)
add_subdirectory(${SOUNDTOUCH_DIR})


# RUBBERBAND
set(RUBBERBAND_DIR ${CANTINA_THIRDPARTY_DIR}/rubber-band)
set(RUBBERBAND_LIB_DIR ${RUBBERBAND_DIR}/lib)
# bad bad bad
set(RUBBERBAND_LIB ${RUBBERBAND_LIB_DIR}/librubberband.a)


# internal

set(HELMHOLTZ_DIR ${CANTINA_LIB_DIR}/helmholtz_tilde)
add_subdirectory(${HELMHOLTZ_DIR})

# finally, the actual flags.
set(CMAKE_CXX_FLAGS "-Wall -Werror -fPIC")

set(PANTOUFLE_DIR ${PROJECT_SOURCE_DIR}/pantoufle)
add_subdirectory(${PANTOUFLE_DIR})

set(CANTINA_COMMON_DIR ${PANTOUFLE_DIR}/cantina_common)

include_directories(
        ${CANTINA_INCLUDE_DIR}
        ${CANTINA_COMMON_DIR}/include
        ${PANTOUFLE_DIR}/include
        # external
        ${SOUNDTOUCH_DIR}/include
        ${HELMHOLTZ_DIR}/include
        ${RUBBERBAND_DIR}/rubberband
        ${FFW3_DIR}/api
)


set(CANTINA_LIB_FILES
        # Cantina lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/Cantina.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/Cantina.hpp
        ${CANTINA_INCLUDE_DIR}/cant/cant.hpp

        #fft lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/fft/FFTWPerformer.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/fft/FFTWPerformer.hpp

        # track lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/track/HelmholtzTracker.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/track/track.hpp
        ${CANTINA_INCLUDE_DIR}/cant/track/PitchTracker.hpp
        ${CANTINA_INCLUDE_DIR}/cant/track/HelmholtzTracker.hpp

        # shift lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/shift/PitchShifter.cpp
        ${CANTINA_SOURCE_DIR}/cant/shift/TimeDomainPitchShifter.cpp
        ${CANTINA_SOURCE_DIR}/cant/shift/MixedDomainPitchShifter.cpp
        ${CANTINA_SOURCE_DIR}/cant/shift/SoundTouchShifter.cpp
        ${CANTINA_SOURCE_DIR}/cant/shift/RubberTouchShifter.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/shift/shift.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/PitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/TimeDomainPitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/MixedDomainPitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/SoundTouchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/RubberTouchShifter.hpp
        )

set(CANTINA_DEP_LIBS
        pantoufle
        # fft, with 32-bit float sample support
        fftw3
        fftw3f 
        # pitch-trackers
        helmholtz
        # pitch-shifters
        soundtouch
        ${RUBBERBAND_LIB}
        )

add_library(cantina ${CANTINA_LIB_FILES})

target_link_libraries(cantina  ${CANTINA_DEP_LIBS})

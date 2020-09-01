cmake_minimum_required(VERSION 3.6)

project(cantina)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

set(CANTINA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(CANTINA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(CANTINA_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(THIRDPARTY_PROXY_DIR ${PROJECT_SOURCE_DIR}/third-party_proxy)

set(CMAKE_CXX_STANDARD 20)

# FLAGS FOR LIBS
set(CMAKE_CXX_FLAGS "-fPIC")

#fftw
set(FFTW3_PROXY_DIR ${THIRDPARTY_PROXY_DIR}/fftw-3.3.8)
add_subdirectory(${FFTW3_PROXY_DIR})

# soundtouch
set(SOUNDTOUCH_PROXY_DIR ${THIRDPARTY_PROXY_DIR}/soundtouch)
add_subdirectory(${SOUNDTOUCH_PROXY_DIR})


# RUBBERBAND
#set(RUBBERBAND_DIR ${THIRDPARTY_DIR}/rubber-band)
#set(RUBBERBAND_LIB_DIR ${RUBBERBAND_DIR}/lib)
# bad bad bad
#set(RUBBERBAND_LIBRARIES ${RUBBERBAND_LIB_DIR}/librubberband.a)

# internal
set(PANTOUFLE_DIR ${PROJECT_SOURCE_DIR}/pantoufle)

# finally, the actual flags.
set(CMAKE_CXX_FLAGS "-Wall -Werror -fPIC")
# add before so that cantina can use pantoufle_include_dirs
add_subdirectory(${PANTOUFLE_DIR})

set(CANTINA_INCLUDE_DIRS
        ${CANTINA_INCLUDE_DIR}
        ${PANTOUFLE_INCLUDE_DIRS}
        ${SOUNDTOUCH_INCLUDE_DIRS}
        ${HELMHOLTZ_DIR}/include
        ${RUBBERBAND_DIR}/rubberband
        ${FFTW3_INCLUDE_DIRS}
        CACHE INTERNAL
        "List of include directories for Cantina"
        )

include_directories(
       ${CANTINA_INCLUDE_DIRS}
)

set(CANTINA_FILES
        # Cantina lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/Cantina.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/Cantina.hpp
        ${CANTINA_INCLUDE_DIR}/cant/cant.hpp
        ${CANTINA_INCLUDE_DIR}/cant/cant_forward.hpp

        #fft lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/fft/FFTWPerformer.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/fft/FFTWPerformer.hpp
        ${CANTINA_INCLUDE_DIR}/cant/fft/FFTWPerformer.inl
        ${CANTINA_INCLUDE_DIR}/cant/fft/fft.hpp

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
        #${CANTINA_SOURCE_DIR}/cant/shift/RubberTouchShifter.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/shift/shift.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/PitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/TimeDomainPitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/MixedDomainPitchShifter.hpp
        ${CANTINA_INCLUDE_DIR}/cant/shift/SoundTouchShifter.hpp
        # ${CANTINA_INCLUDE_DIR}/cant/shift/RubberTouchShifter.hpp


        # External libraries
        ## Helmholtz pitch-tracker by Katja Vetter
        ### source
        ${CANTINA_SOURCE_DIR}/cant/extern/helmholtz/Helmholtz.cpp
        ### header
        ${CANTINA_INCLUDE_DIR}/cant/extern/helmholtz/Helmholtz.hpp
        )

set(CANTINA_DEP_LIBS
        ${PANTOUFLE_LIBRARIES}
        #pitch-trackers
        # pitch-shifters
        ${SOUNDTOUCH_LIBRARIES}
        #${RUBBERBAND_LIBRARIES}
        # fftw, with 32-bit float sample support
        # fftw3f
        ${FFTW3_LIBRARIES}
        )


add_library(cantina STATIC ${CANTINA_FILES})

set(CANTINA_LIBRARIES cantina ${CANTINA_DEP_LIBS} PARENT_SCOPE)


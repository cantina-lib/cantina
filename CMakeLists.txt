cmake_minimum_required(VERSION 3.12)

project(cantina CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules/cmake)

set(CANTINA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(CANTINA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(CANTINA_INLINE_DIR ${PROJECT_SOURCE_DIR}/inline)


set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third-party)
set(THIRDPARTY_PROXY_DIR ${PROJECT_SOURCE_DIR}/third-party_proxy)

set(PANTOUFLE_DIR ${PROJECT_SOURCE_DIR}/pantoufle)
set(SOUNDTOUCH_PROXY_DIR ${THIRDPARTY_PROXY_DIR}/soundtouch)

# subdirs
add_subdirectory(${PANTOUFLE_DIR})

#FFTW
set(FFTW_USE_STATIC_LIBS TRUE)
find_package(FFTW REQUIRED)
message(STATUS "Found FFTW: " ${FFTW_LIBRARIES})

# Soundtouch
# no static version of Soundtouch in deb by default, whatever.
add_subdirectory(${SOUNDTOUCH_PROXY_DIR})

set(CANTINA_INCLUDE_DIRS
        ${CANTINA_INCLUDE_DIR}
        ${CANTINA_INLINE_DIR}
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
        ${CANTINA_INCLUDE_DIR}/cant/fft/FFTWAllocator.hpp
        ${CANTINA_INCLUDE_DIR}/cant/fft/fft.hpp

        # track lib
        ## source
        ${CANTINA_SOURCE_DIR}/cant/track/HelmholtzTracker.cpp
        ## headers
        ${CANTINA_INCLUDE_DIR}/cant/track/track.hpp
        ${CANTINA_INCLUDE_DIR}/cant/track/Pitch.hpp
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

add_library(${PROJECT_NAME} STATIC ${CANTINA_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE ${CANTINA_CXX_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE ${CANTINA_CXX_STANDARD})

target_include_directories(${PROJECT_NAME} PUBLIC ${CANTINA_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${PANTOUFLE_LIBRARIES}
        FFTW::Float
        ${SOUNDTOUCH_LIBRARIES}
        )

set(CANTINA_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)


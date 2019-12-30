cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

project (qsp)

set(SOURCES
    src/actions.cpp
    src/callbacks.cpp
    src/codetools.cpp
    src/coding.cpp
    src/common.cpp
    src/errors.cpp
    src/game.cpp
    src/jack.cpp
    src/locations.cpp
    src/mathops.cpp
    src/menu.cpp
    src/objects.cpp
    src/playlist.cpp
    src/regexp.cpp
    src/statements.cpp
    src/text.cpp
    src/time_qsp.cpp
    src/towlower.cpp
    src/towupper.cpp
    src/variables.cpp
    src/variant.cpp
    src/android/android_callbacks.cpp
    src/android/android_coding.cpp
    src/android/android_control.cpp
    src/default/default_callbacks.cpp
    src/default/default_coding.cpp
    src/default/default_control.cpp
    src/flash/flash_callbacks.cpp
    src/flash/flash_coding.cpp
    src/flash/flash_control.cpp
    src/java/java_callbacks.cpp
    src/java/java_coding.cpp
    src/java/java_control.cpp
)

set(HEADERS
    src/actions.h
    src/callbacks.h
    src/codetools.h
    src/coding.h
    src/common.h
    src/declarations.h
    src/errors.h
    src/game.h
    src/locations.h
    src/mathops.h
    src/menu.h
    src/objects.h
    src/playlist.h
    src/regexp.h
    src/statements.h
    src/text.h
    src/time_qsp.h
    src/variables.h
    src/variant.h
)

IF (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WIN)
ELSE()
    add_compile_definitions(NOT_RUBY)
ENDIF()

add_compile_definitions(_UNICODE EXPORT)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

add_library(FastQsp::qsp ALIAS ${PROJECT_NAME})

find_package(oniguruma CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core
                                      PRIVATE oniguruma::onig )


set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS qsp DESTINATION bin/)
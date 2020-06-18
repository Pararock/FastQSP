cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(qsp VERSION 5.8.0 )

set(SOURCES
    actions.cpp
    callbacks.cpp
    codetools.cpp
    coding.cpp
    common.cpp
    errors.cpp
    game.cpp
    jack.cpp
    locations.cpp
    mathops.cpp
    menu.cpp
    objects.cpp
    playlist.cpp
    regexp.cpp
    statements.cpp
    text.cpp
    time_qsp.cpp
    towlower.cpp
    towupper.cpp
    variables.cpp
    variant.cpp
    bindings/android/android_callbacks.cpp
    bindings/android/android_coding.cpp
    bindings/android/android_control.cpp
    bindings/default/default_callbacks.cpp
    bindings/default/default_coding.cpp
    bindings/default/default_control.cpp
    bindings/flash/flash_callbacks.cpp
    bindings/flash/flash_coding.cpp
    bindings/flash/flash_control.cpp
    bindings/java/java_callbacks.cpp
    bindings/java/java_coding.cpp
    bindings/java/java_control.cpp
)

set(HEADERS
    bindings/android/android.h
    bindings/default/qsp_default.h
    bindings/flash/flash.h
    bindings/java/java.h
    bindings/bindings_config.h
    actions.h
    callbacks.h
    codetools.h
    coding.h
    common.h
    declarations.h
    errors.h
    game.h
    jack.h
    locations.h
    mathops.h
    menu.h
    objects.h
    playlist.h
    qsp.h
    regexp.h
    statements.h
    text.h
    time_qsp.h
    variables.h
    variant.h
)


set(DEFINES NOT_RUBY EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS)

add_library(qsp ${SOURCES} ${HEADERS})
find_package(oniguruma CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core
                                      PRIVATE oniguruma::onig )

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
    INCLUDE_DIRECTORIES "${INCLUDES}"
)

install(TARGETS qsp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}) 
cmake_minimum_required(VERSION 3.0.2)


if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()


project(FastQsp CXX)
set(SOURCES
    main.cpp
    qsp_callback.cpp
    fastqspwindow.cpp
    qsp_htmlbuilder.cpp
    local_requsts_proxy.cpp
    audiostream.cpp
    #QspExecWebEngineUrlSchemeHandler.cpp
    #QspWebEngineUrlRequestInterceptor.cpp
    #QspWebEngineUrlSchemeHandler.cpp
)

set(HEADERS
    qsp.h
    qsp_callback.h
    qsp_default.h
    fastqspwindow.h
    qsp_htmlbuilder.h
    local_requsts_proxy.h
    audiostream.h
    #QspExecWebEngineUrlSchemeHandler.cpp
    #QspWebEngineUrlRequestInterceptor.cpp
    #QspWebEngineUrlSchemeHandler.cpp
)

set(DEFINES NOT_RUBY _UNICODE GIT_VERSION=${GIT_VERSION})
if (WIN32)
    list(APPEND DEFINES _WIN)
endif(WIN32)
set(INCLUDES
    ${PROJECT_SOURCE_DIR}
    #todo figure out export .h with library cmake
    ${CMAKE_SOURCE_DIR}/qsp
)
set(LIBRARIES qsp)

#find_package(oniguruma CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} oniguruma)

# Qt stuff
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Network Widgets Multimedia MultimediaWidgets WebEngine WebEngineWidgets)# WebKit WebKitWidgets)
if (Qt5_FOUND)
    message(STATUS "Using Qt5")
    list(APPEND INCLUDES
        ${Qt5Core_INCLUDE_DIRS}
        #${Qt5Gui_INCLUDE_DIRS}
        #${Qt5Network_INCLUDE_DIRS}
        #${Qt5Multimedia_INCLUDE_DIRS}
        #${Qt5WebKit_INCLUDE_DIRS}
        #${Qt5WebKitWidgets_INCLUDE_DIRS}
    )
    list(APPEND LIBRARIES
        ${Qt5Core_LIBRARIES}
        #${Qt5Gui_LIBRARIES}
        #${Qt5Network_LIBRARIES}
        #${Qt5Multimedia_LIBRARIES}
        #${Qt5WebKit_LIBRARIES}
        #${Qt5WebKitWidgets_LIBRARIES}
    )
    list(REMOVE_DUPLICATES INCLUDES)
    list(REMOVE_DUPLICATES LIBRARIES)
else()
    message(STATUS "Qt5 is not found fallback to Qt4")
    find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtWebKit REQUIRED)
    find_package(Phonon REQUIRED)
    list(APPEND INCLUDES
        ${QT_INCLUDES}
        ${PHONON_INCLUDES}
    )
    list(APPEND LIBRARIES
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTNETWORK_LIBRARY}
        ${QT_QTWEBKIT_LIBRARY}
        ${PHONON_LIBRARIES}
    )
    list(REMOVE_DUPLICATES INCLUDES)
    list(REMOVE_DUPLICATES LIBRARIES)
endif(Qt5_FOUND)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
    INCLUDE_DIRECTORIES "${INCLUDES}"
)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

target_link_libraries(FastQsp qsp Qt5::Core Qt5::Network Qt5::Widgets  Qt5::Multimedia Qt5::MultimediaWidgets Qt5::WebEngine Qt5::WebEngineWidgets) #Qt5::Gui)


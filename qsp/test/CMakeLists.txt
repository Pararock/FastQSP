find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
enable_testing()


add_executable(
    runUnitTests
    main.cpp
    game_tests.cpp
    variant_tests.cpp)

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(runUnitTests 
    qsp GTest::gtest GTest::gmock
    )

target_compile_features(runUnitTests PRIVATE cxx_std_17) 
IF (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WIN)
ELSE()
    add_compile_definitions(NOT_RUBY)
ENDIF()
add_compile_definitions(_UNICODE)


# should probably move this as post build if assets change
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

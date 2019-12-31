find_package(GTest CONFIG REQUIRED)

enable_testing()

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
    runUnitTests
    #main.cpp
    game_tests.cpp
    )

#/target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(
    runUnitTests 
    GTest::gtest GTest::gtest_main #GTest::gmock GTest::gmock_main
    )

target_link_libraries(runUnitTests qsp)
target_compile_features(runUnitTests  PRIVATE cxx_std_17) 

IF (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WIN)
ELSE()
    add_compile_definitions(NOT_RUBY)
ENDIF()
add_compile_definitions(_UNICODE)
#
#add_test(
  #NAME
    #unit
  #COMMAND
    #${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  #)


#target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME that-test-I-made COMMAND unit_tests)

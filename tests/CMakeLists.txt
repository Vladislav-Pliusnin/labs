cmake_minimum_required(VERSION 3.24)
project(WarehouseTests LANGUAGES CXX)
include(CTest)
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0)
FetchContent_MakeAvailable(googletest)
add_executable(types_test TypesTest.cpp RobotTest.cpp ShelfTest.cpp OrderTest.cpp)
target_compile_features(types_test PRIVATE cxx_std_23)
target_link_libraries(types_test PRIVATE GTest::gtest_main)
target_include_directories(types_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
include(GoogleTest)
gtest_discover_tests(types_test DISCOVERY_MODE PRE_TEST)
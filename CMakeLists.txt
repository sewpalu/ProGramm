cmake_minimum_required(VERSION 3.13)

project(form_lang VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/depend/docopt)
add_subdirectory(${PROJECT_SOURCE_DIR}/depend/wxWidgets)

file(GLOB ${PROJECT_NAME}_src "src/*")

##
## BUILD
##

add_library(${PROJECT_NAME}lib STATIC ${${PROJECT_NAME}_src})
target_include_directories(${PROJECT_NAME}lib PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}lib docopt_s wx::net wx::core wx::base)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}lib)

##
## TESTS
##

macro(form_lang_add_test TEST_NAME)
  add_executable(${TEST_NAME} tests/${TEST_NAME}.cpp)
  target_include_directories(${TEST_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${gtest_SOURCE_DIR}/include)
  target_link_libraries(${TEST_NAME}
    ${PROJECT_NAME}lib
    gtest gmock gtest_main)
  gtest_discover_tests(${TEST_NAME})
endmacro()

option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
  add_subdirectory(${PROJECT_SOURCE_DIR}/depend/googletest)
  enable_testing()
  include(GoogleTest)

  form_lang_add_test(dummy_test)
endif()


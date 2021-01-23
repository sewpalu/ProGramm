cmake_minimum_required(VERSION 3.1)

project(form_lang VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(depend/docopt)

file(GLOB ${PROJECT_NAME}_src "src/*")

add_executable(${PROJECT_NAME} main.cpp ${${PROJECT_NAME}_src})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} docopt)


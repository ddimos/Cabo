cmake_minimum_required(VERSION 3.24)
project(Cabo VERSION 0.1.0)

find_package(SFML 2.6 COMPONENTS graphics system window network REQUIRED)

# include(FetchContent)

# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x)
# FetchContent_MakeAvailable(SFML)


set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TARGET_SRC "./src/cabo/*.cpp" )

add_executable(Cabo ${TARGET_SRC} "./src/main.cpp")

target_link_libraries(Cabo sfml-graphics sfml-system sfml-window sfml-network)

target_include_directories(Cabo PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/cabo"
                            )

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

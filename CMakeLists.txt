cmake_minimum_required(VERSION 3.24)
project(Cabo VERSION 0.1.0)

find_package(SFML 2.6 COMPONENTS graphics system window network REQUIRED)

include(FetchContent)

# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x)
# FetchContent_MakeAvailable(SFML)

FetchContent_Declare(nsf
    GIT_REPOSITORY https://github.com/ddimos/nsf.git
    GIT_TAG v0.1.1)

option(NSF_BUILD_EXAMPLE "Build the nsf example" OFF)
FetchContent_MakeAvailable(nsf)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TARGET_SRC "./src/cabo/*.cpp" )

add_executable(Cabo ${TARGET_SRC} "./src/main.cpp")

target_link_libraries(Cabo sfml-graphics sfml-system sfml-window sfml-network nsf)

target_include_directories(Cabo PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/cabo"
                            )

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(LAUNCH_TYPE "DEV" CACHE STRING "Set the launch type. Possible values [CLIENT, DEV, SERVER]")

message("Generated launch type: ${LAUNCH_TYPE}")

if (LAUNCH_TYPE STREQUAL "CLIENT")
    set(TARGET_OUTPUT_NAME "Cabo_Client")
elseif (LAUNCH_TYPE STREQUAL "DEV")
    set(TARGET_OUTPUT_NAME "Cabo_Dev")
elseif (LAUNCH_TYPE STREQUAL "SERVER")
    set(TARGET_OUTPUT_NAME "Cabo_Server")
else ()
    message(FATAL_ERROR "LAUNCH_TYPE should be equal to CLIENT|DEV|SERVER, current value is ${LAUNCH_TYPE}")
endif()

target_compile_definitions(Cabo PRIVATE
    $<$<STREQUAL:${LAUNCH_TYPE},CLIENT>:CN_LAUNCH_TYPE=CN_LAUNCH_TYPE_CLIENT>
    $<$<STREQUAL:${LAUNCH_TYPE},DEV>:CN_LAUNCH_TYPE=CN_LAUNCH_TYPE_DEV>
    $<$<STREQUAL:${LAUNCH_TYPE},SERVER>:CN_LAUNCH_TYPE=CN_LAUNCH_TYPE_SERVER>
)

message("Generated executable name: ${TARGET_OUTPUT_NAME}")

set_target_properties(
    Cabo 
    PROPERTIES 
        OUTPUT_NAME "${TARGET_OUTPUT_NAME}"
)

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
    GIT_TAG v0.2.0)

option(NSF_BUILD_EXAMPLE "Build the nsf example" OFF)
FetchContent_MakeAvailable(nsf)

set(CMAKE_CXX_STANDARD 20)

set(LAUNCH_TARGET "Client" CACHE STRING "Set the launch target. Possible values [Client, Server]")

message("Generated launch type: ${LAUNCH_TARGET}")

if (LAUNCH_TARGET STREQUAL "Client")
    file(GLOB_RECURSE TARGET_SRC 
                                "./src/cabo/client/*.cpp"
                                "./src/cabo/core/*.cpp"
                                "./src/cabo/shared/*.cpp"
                                "./src/cabo/ContextObjectIds.cpp"
    )
else ()
    file(GLOB_RECURSE TARGET_SRC 
                                "./src/cabo/server/*.cpp"
                                "./src/cabo/core/*.cpp"
                                "./src/cabo/shared/*.cpp"
                                "./src/cabo/ContextObjectIds.cpp"
    )
endif ()

add_executable(Cabo ${TARGET_SRC} "./src/main.cpp")

target_link_libraries(Cabo sfml-graphics sfml-system sfml-window sfml-network nsf)

target_include_directories(Cabo PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/cabo"
                            )

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if (LAUNCH_TARGET STREQUAL "Client")
    set(TARGET_OUTPUT_NAME "Cabo_Client")
elseif (LAUNCH_TARGET STREQUAL "Server")
    set(TARGET_OUTPUT_NAME "Cabo_Server")
else ()
    message(FATAL_ERROR "LAUNCH_TARGET should be equal to Client|Server, current value is ${LAUNCH_TARGET}")
endif()

target_compile_definitions(Cabo PRIVATE
    CN_LAUNCH_TARGET_CLIENT=1
    CN_LAUNCH_TARGET_SERVER=2
    $<$<STREQUAL:${LAUNCH_TARGET},Client>:CN_LAUNCH_TARGET=CN_LAUNCH_TARGET_CLIENT>
    $<$<STREQUAL:${LAUNCH_TARGET},Server>:CN_LAUNCH_TARGET=CN_LAUNCH_TARGET_SERVER>
)

message("Generated executable name: ${TARGET_OUTPUT_NAME}")

set_target_properties(
    Cabo 
    PROPERTIES 
        OUTPUT_NAME "${TARGET_OUTPUT_NAME}"
)

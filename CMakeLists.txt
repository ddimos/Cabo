cmake_minimum_required(VERSION 3.24)
project(Cabo VERSION 0.1.0)


include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

#find_package(SFML 2.6 COMPONENTS graphics system window network REQUIRED)

FetchContent_Declare(nsf
    GIT_REPOSITORY https://github.com/ddimos/nsf.git
    GIT_TAG v0.2.4)

set(NSF_FIND_SFML OFF CACHE BOOL "Use find_package to find SFML")
set(NSF_BUILD_EXAMPLE OFF CACHE BOOL "Build the nsf example")
set(NSF_LOCAL_BUILD OFF CACHE BOOL "Disable local build")

FetchContent_MakeAvailable(nsf)

set(CMAKE_CXX_STANDARD 20)

set(LAUNCH_TARGET "Client" CACHE STRING "Set the launch target. Possible values [Client, Server]")
set(DISABLE_ASSERT false CACHE BOOL "Disable asserts.")
set(DISABLE_LOGGING false CACHE BOOL "Disable logging.")

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

file(GLOB_RECURSE RESOURCES_FONTS 
    "${CMAKE_SOURCE_DIR}/res/fonts/*.ttf"
)
file(GLOB_RECURSE RESOURCES_SAVES 
    "${CMAKE_SOURCE_DIR}/res/save/*"
)
file(GLOB_RECURSE RESOURCES_TEXTURES
    "${CMAKE_SOURCE_DIR}/res/textures/*.png"
)

add_executable(Cabo ${TARGET_SRC} 
                    "./src/main.cpp"
                    ${RESOURCES_FONTS}
                    ${RESOURCES_SAVES}
                    ${RESOURCES_TEXTURES}
)

target_link_libraries(Cabo sfml-graphics sfml-system sfml-window sfml-network nsf)

target_include_directories(Cabo PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/cabo"
                            )

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if (LAUNCH_TARGET STREQUAL "Client")
    set(TARGET_OUTPUT_NAME "Cabo")
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

# TODO disable asserts and logs in Release
if (DISABLE_ASSERT)
    target_compile_definitions(Cabo PRIVATE CN_DISABLE_ASSERT=1)
    message("Dissable asserts")
endif()

if (DISABLE_LOGGING)
    target_compile_definitions(Cabo PRIVATE CN_DISABLE_LOGGING=1)
    message("Dissable logging")
endif()

message("Generated executable name: ${TARGET_OUTPUT_NAME}")

set_target_properties(
    Cabo 
    PROPERTIES 
        OUTPUT_NAME "${TARGET_OUTPUT_NAME}"
)

# Taken from https://ryouze.net/posts/sfml-cmake-macos/
if(APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.ddimos.${PROJECT_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})

    configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist @ONLY)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    set_source_files_properties(${RESOURCES_FONTS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/fonts")
    set_source_files_properties(${RESOURCES_SAVES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/save")
    set_source_files_properties(${RESOURCES_TEXTURES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/textures")

    # Clean up the Frameworks directory before copying
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks
        COMMENT "Cleaning Frameworks directory"
    )

    # Copy all frameworks into the app bundle
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #     COMMAND rsync -a ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/
    #         $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks/
    #     COMMENT "Copying all SFML frameworks into the app bundle"
    # )

    # Copy only the SFML freetype framework into the app bundle
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND rsync -a ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/freetype.framework
            $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks/
        COMMENT "Copying SFML freetype framework into the app bundle"
    )

    # Add install target for macOS app bundle
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION /Applications)
else()
    # Add install target for regular executable
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

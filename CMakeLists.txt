cmake_minimum_required(VERSION 3.25)

project(SDL_Pelda LANGUAGES C)

set(CMAKE_C_STANDARD 11)

if(CMAKE_VERSION VERSION_GREATER "3.29.0")
    cmake_policy(SET CMP0156 NEW)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Wconversion)
endif ()

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/")
endif ()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/TC /W4 /permissive-)
endif ()

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

if(NOT WIN32)
    add_library(SDL2_gfx SHARED
            external/GFX_SOURCE/SDL2_framerate.c
            external/GFX_SOURCE/SDL2_gfxPrimitives.c
            external/GFX_SOURCE/SDL2_gfxPrimitives.h
            external/GFX_SOURCE/SDL2_gfxPrimitives_font.h
            external/GFX_SOURCE/SDL2_imageFilter.c
            external/GFX_SOURCE/SDL2_imageFilter.h
            external/GFX_SOURCE/SDL2_rotozoom.c
            external/GFX_SOURCE/SDL2_rotozoom.h)

    target_link_libraries(SDL2_gfx PRIVATE SDL2::SDL2main PRIVATE SDL2::SDL2)
endif ()

if(MSVC)
    add_library(SDL2_gfx STATIC
            external/GFX_SOURCE/SDL2_framerate.c
            external/GFX_SOURCE/SDL2_gfxPrimitives.c
            external/GFX_SOURCE/SDL2_gfxPrimitives.h
            external/GFX_SOURCE/SDL2_gfxPrimitives_font.h
            external/GFX_SOURCE/SDL2_imageFilter.c
            external/GFX_SOURCE/SDL2_imageFilter.h
            external/GFX_SOURCE/SDL2_rotozoom.c
            external/GFX_SOURCE/SDL2_rotozoom.h)

    target_link_libraries(SDL2_gfx PRIVATE SDL2::SDL2main PRIVATE SDL2::SDL2)
endif ()

add_executable(SDL_Pelda main.c)

target_link_libraries(SDL_Pelda PRIVATE SDL2::SDL2main PRIVATE SDL2::SDL2 PRIVATE SDL2_image::SDL2_image PRIVATE SDL2_ttf::SDL2_ttf PRIVATE SDL2_mixer::SDL2_mixer)

if(WIN32 AND NOT MSVC)
    find_package(SDL2_gfx REQUIRED)
    target_link_libraries(SDL_Pelda PRIVATE SDL2_gfx::SDL2_gfx)
else ()
    target_include_directories(SDL_Pelda PRIVATE external/GFX_SOURCE)
    target_link_libraries(SDL_Pelda PRIVATE SDL2_gfx)
endif ()


if (LINUX)
    target_link_libraries(SDL_Pelda PRIVATE m)
endif ()

file(GLOB_RECURSE ASSETS_ "assets/*.*")

if(NOT ASSETS_ STREQUAL "")
    file(COPY ${ASSETS_} DESTINATION ${CMAKE_BINARY_DIR})
endif ()

if(WIN32)
    add_custom_command(TARGET SDL_Pelda POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:SDL_Pelda> $<TARGET_RUNTIME_DLLS:SDL_Pelda>
            COMMAND_EXPAND_LISTS
    )
endif (WIN32)

#CMake minimum verziót itt állíthatjuk be. Debian miatt van ilyen alacsonyan, ott még a 3.25 verzió van az apt-ben :(
cmake_minimum_required(VERSION 3.25)

#projekt nevét és nyelvét(nyelveit) lehet itt beállítani
project(SDL_Pelda LANGUAGES C)

#C11es standardot választunk
#https://en.wikipedia.org/wiki/C_standard
set(CMAKE_C_STANDARD 11)

if(CMAKE_VERSION VERSION_GREATER "3.29.0")
    cmake_policy(SET CMP0156 NEW)
endif()

#ha gcc a compiler akkor mindenféle extra warningot kérünk
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Wconversion)
endif ()

if(WIN32)
    #a CMAKE_PREFIX_PATH mondja meg, hogy hol kell a dependencyket keresni, ez windowson kell nekünk
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/")
endif ()


#ha msvc compileren vagyunk akkor bekapcsolunk plusz warningokat és a nonstandard dolgokra is warningolunk, valamint beállítjuk, hogy C kódként forduljon minden
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/TC /W4 /permissive-)
endif ()

#megkeressük az SDL2 packageket (CONFIG módban keressük az SDL-t, mert config fileokat ad)
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

if(NOT WIN32) #nem windowson source-ból buildeljük az SDL2_gfx-et
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

if(MSVC) #MSVC prebuilt binary nincs, szóval source-ból buildeljük az SDL2_gfx-et. Azért static, mert kézzel kéne dllexportolni minden gfx-es függvényt
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

#Példa executable, egy forrásfile-al
add_executable(SDL_Pelda main.c)

#Linkeljük a könyvtárakat, itt az SDL2main mindenképp előbb kell legyen mint a többi SDL könyvtár
target_link_libraries(SDL_Pelda PRIVATE SDL2::SDL2main PRIVATE SDL2::SDL2 PRIVATE SDL2_image::SDL2_image PRIVATE SDL2_ttf::SDL2_ttf PRIVATE SDL2_mixer::SDL2_mixer)

#sajnos MSVC-vel nem fog működni az SDL2_gfx ebben a scriptben, prebuilt binary nincs hozzá, sem rendes cmake config :(

if(WIN32 AND NOT MSVC) #windowson pedig a custom cmake scriptet használjuk
    find_package(SDL2_gfx REQUIRED)
    target_link_libraries(SDL_Pelda PRIVATE SDL2_gfx::SDL2_gfx)
else () #nem windowson pedig a buildelt gfx-et használjuk
    target_include_directories(SDL_Pelda PRIVATE external/GFX_SOURCE)
    target_link_libraries(SDL_Pelda PRIVATE SDL2_gfx)
endif ()


if (LINUX)
    #linuxon külön kell linkelni a math libraryt
    target_link_libraries(SDL_Pelda PRIVATE m)
endif ()

#az assets mappából minden filet összegyűjtünk egy ASSETS_ változó alá
file(GLOB_RECURSE ASSETS_ "assets/*.*")

#az ASSETS_ -ben található fileokat bemásoljuk a programunk mellé
if(NOT ASSETS_ STREQUAL "")
    file(COPY ${ASSETS_} DESTINATION ${CMAKE_BINARY_DIR})
endif ()

#Windowson ez a szörnyeteg másolja a szükséges dll-eket(azokat, amelyekről a CMake is tud) az executable mellé
if(WIN32)
    add_custom_command(TARGET SDL_Pelda POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:SDL_Pelda> $<TARGET_RUNTIME_DLLS:SDL_Pelda>
            COMMAND_EXPAND_LISTS
    )
endif (WIN32)

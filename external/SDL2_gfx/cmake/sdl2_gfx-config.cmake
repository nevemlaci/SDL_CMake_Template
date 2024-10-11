add_library(SDL2_gfx::SDL2_gfx STATIC IMPORTED)
    set_target_properties(SDL2_gfx::SDL2_gfx
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../include"
            IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/../lib/libSDL2_gfx.a"
            IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../bin/SDL2_gfx.dll"
    )
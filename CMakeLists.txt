cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Game)

include(FetchContent)

set(libgfx_local true)

if(libgfx_local)
    add_subdirectory(libgfx)
else()
    FetchContent_Declare(
    Gfx
    GIT_REPOSITORY https://github.com/lukasx999/libgfx.git
    GIT_TAG main
    )
    FetchContent_MakeAvailable(Gfx)
endif()

FetchContent_Declare(
    Tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG master
    SOURCE_SUBDIR tmxlite
)
FetchContent_MakeAvailable(Tmxlite)

add_executable(game main.cc)
target_link_libraries(game PRIVATE gfx tmxlite)

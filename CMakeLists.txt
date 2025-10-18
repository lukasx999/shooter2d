cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Game)

include(FetchContent)

FetchContent_Declare(
    Gfx
    GIT_REPOSITORY /home/lukas/code/repos/libgfx/
    # GIT_REPOSITORY https://github.com/lukasx999/libgfx.git
    GIT_TAG main
)

FetchContent_MakeAvailable(Gfx)

add_executable(game main.cc)
target_link_libraries(game PRIVATE gfx)

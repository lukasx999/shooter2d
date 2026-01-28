cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(Game)

find_package(gfx)

include(FetchContent)

FetchContent_Declare(
    Tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG master
    SOURCE_SUBDIR tmxlite
)
FetchContent_MakeAvailable(Tmxlite)

add_executable(game)
target_compile_options(game PRIVATE -Wall -Wextra -O3 -ggdb)
target_link_libraries(game PRIVATE gfx tmxlite)

add_subdirectory(src)

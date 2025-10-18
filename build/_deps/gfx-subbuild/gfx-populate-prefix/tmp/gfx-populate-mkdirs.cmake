# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/lukas/code/repos/shooter2d/build/_deps/gfx-src")
  file(MAKE_DIRECTORY "/home/lukas/code/repos/shooter2d/build/_deps/gfx-src")
endif()
file(MAKE_DIRECTORY
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-build"
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix"
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/tmp"
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/src/gfx-populate-stamp"
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/src"
  "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/src/gfx-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/src/gfx-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lukas/code/repos/shooter2d/build/_deps/gfx-subbuild/gfx-populate-prefix/src/gfx-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

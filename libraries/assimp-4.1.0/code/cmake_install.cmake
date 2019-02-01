# Install script for directory: /Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/Kleinman/Documents/Programming/C:C++/Next/lib/libassimpd.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/anim.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/ai_assert.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/camera.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/color4.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/color4.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/config.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/defs.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Defines.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/cfileio.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/light.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/material.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/material.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/matrix3x3.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/matrix3x3.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/matrix4x4.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/matrix4x4.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/mesh.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/postprocess.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/quaternion.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/quaternion.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/scene.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/metadata.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/texture.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/types.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/vector2.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/vector2.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/vector3.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/vector3.inl"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/version.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/cimport.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/importerdesc.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Importer.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/DefaultLogger.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/ProgressHandler.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/IOStream.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/IOSystem.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Logger.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/LogStream.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/NullLogger.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/cexport.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Exporter.hpp"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/DefaultIOStream.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/DefaultIOSystem.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/SceneCombiner.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Compiler/pushpack1.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Compiler/poppack1.h"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/../include/assimp/Compiler/pstdint.h"
    )
endif()


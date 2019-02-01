# Install script for directory: /Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp4.1.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/assimp-4.1" TYPE FILE FILES
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/assimp-config.cmake"
    "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/assimp-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibassimp4.1.0-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/assimp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/contrib/cmake_install.cmake")
  include("/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/code/cmake_install.cmake")
  include("/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/tools/assimp_cmd/cmake_install.cmake")
  include("/Users/Kleinman/Documents/Programming/C:C++/Next/libraries/assimp-4.1.0/test/cmake_install.cmake")

endif()


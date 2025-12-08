# Install script for directory: /home/rei/Documents/Code/CS407-Project/vendored/harfbuzz

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-aat-layout.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-aat.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-blob.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-buffer.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-common.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-cplusplus.hh"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-deprecated.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-draw.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-face.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-font.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-map.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-color.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-deprecated.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-font.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-layout.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-math.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-meta.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-metrics.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-name.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-shape.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot-var.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-ot.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-paint.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-set.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-script-list.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-shape-plan.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-shape.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-style.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-unicode.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-version.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-subset.h"
    "/home/rei/Documents/Code/CS407-Project/vendored/harfbuzz/src/hb-subset-serialize.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/libharfbuzz.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "pkgconfig" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/harfbuzz.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz/harfbuzzConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz/harfbuzzConfig.cmake"
         "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/CMakeFiles/Export/6988f0906c47366608790bc51d4c19aa/harfbuzzConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz/harfbuzzConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz/harfbuzzConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz" TYPE FILE FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/CMakeFiles/Export/6988f0906c47366608790bc51d4c19aa/harfbuzzConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/harfbuzz" TYPE FILE FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/CMakeFiles/Export/6988f0906c47366608790bc51d4c19aa/harfbuzzConfig-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/libharfbuzz-subset.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so"
         OLD_RPATH "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libharfbuzz-subset.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "pkgconfig" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/harfbuzz-subset.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/src/hb-features.h")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/rei/Documents/Code/CS407-Project/build/vendored/harfbuzz/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()

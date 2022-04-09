#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw
  /Applications/CMake.app/Contents/bin/cmake -P /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw
  /Applications/CMake.app/Contents/bin/cmake -P /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw
  /Applications/CMake.app/Contents/bin/cmake -P /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw
  /Applications/CMake.app/Contents/bin/cmake -P /Users/qianzecheng/Code/5607/hw2a/bin/ext/glfw/cmake_uninstall.cmake
fi


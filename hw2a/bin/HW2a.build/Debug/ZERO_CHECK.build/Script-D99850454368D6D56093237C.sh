#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin
  make -f /Users/qianzecheng/Code/5607/hw2a/bin/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin
  make -f /Users/qianzecheng/Code/5607/hw2a/bin/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin
  make -f /Users/qianzecheng/Code/5607/hw2a/bin/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/qianzecheng/Code/5607/hw2a/bin
  make -f /Users/qianzecheng/Code/5607/hw2a/bin/CMakeScripts/ReRunCMake.make
fi


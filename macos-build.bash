#! /usr/bin/bash

set -e

[ ! -d "build" ] && mkdir build
pushd build 

# INFO(Ryan): -DLL_FAST -DLL_RELEASE -Ofast -pipe
ll_common_compiler_flags="-DLL_SLOW -DLL_DEV -fno-omit-frame-pointer -fno-optimize-sibling-calls -g
 -fuse-ld=lld -Wall -Wextra -Wpedantic -Wfloat-equal -Wunreachable-code -Wshadow -Wno-gnu"

# INFO(Ryan): -static -fPIC -o ll.a
# INFO(Ryan): clang -shared $ll_common_compiler_flags ..\code\ll.c -o ll.so 

# NOTE(Ryan): The name of the static sdl libraries end with 'd' for debug builds.

# IMPORTANT(Ryan): This uses deprecated frameworks. Needs to be revisited on a modern macosx version.
clang $ll_common_compiler_flags ../code/sdl-ll.c -o sdl-ll \
  -I ../sdl2-2.0.9-minimal/include -L ../sdl2-2.0.9-minimal/build \
  -framework AudioToolbox -framework CoreAudio -framework Carbon \
    -framework ForceFeedback -framework IOKit -framework Cocoa -framework CoreVideo \
  -lm -liconv \
  -lSDL2maind -lSDL2d 

# -L/usr/local/lib -l/System/Library/Frameworks/AudioToolbox.framework -l/System/Library/Frameworks/CoreAudio.framework -l/System/Library/Frameworks/Carbon.framework -l/System/Library/Frameworks/ForceFeedback.framework -l/System/Library/Frameworks/IOKit.framework -l/System/Library/Frameworks/Cocoa.framework -l/System/Library/Frameworks/CoreVideo.framework -liconv -lm -Wl,-current_version,10.0.0 -Wl,-compatibility_version,1.0.0 -Wl,-undefined,error -lSDL2

popd

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
clang $ll_common_compiler_flags ../code/sdl-ll.c -o sdl-ll \
  -I ../sdl2-2.0.9-minimal/include -L ../sdl2-2.0.9-minimal/build \
  -lm -ldl -pthread -lX11 -lXext -lXxf86vm \
  -lSDL2maind -lSDL2d 

# -lX11 -lXext -lpthread -lGL -lpulse \

popd

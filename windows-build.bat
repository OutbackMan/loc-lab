@ECHO OFF

IF NOT EXIST build MKDIR build
PUSHD build 

REM INFO(Ryan): -DLL_FAST -DLL_RELEASE -Ofast -pipe
REM IMPORTANT(Ryan): Requires LLVM 8+ to use lld with mingw libraries.
SET ll_common_compiler_flags=-DLL_SLOW -DLL_DEV -fno-omit-frame-pointer -fno-optimize-sibling-calls -g^
 -target x86_64-windows-gnu -fuse-ld=lld.exe -Wall -Wextra -Wpedantic -Wfloat-equal -Wunreachable-code -Wshadow^
 -Wno-gnu

REM INFO(Ryan): -static -fPIC -o ll.lib
REM INFO(Ryan): clang.exe -shared %ll_common_compiler_flags% ..\code\ll.c -o ll.dll 
REM NOTE(Ryan): The name of the static sdl libraries end with 'd' for debug builds.
clang.exe %ll_common_compiler_flags% ..\code\sdl-ll.c -o sdl-ll.exe^
 -Wl,-subsystem,windows -I ..\sdl2-2.0.9-minimal\include -L ..\sdl2-2.0.9-minimal\build^
  -lmingw32 -lwinmm -lole32 -limm32 -loleaut32 -lgdi32 -luuid -lversion -lopengl32^
    -lSDL2maind -lSDL2d 

POPD

@ECHO OFF

IF NOT EXIST build MKDIR build
PUSHD build 

REM INFO(Ryan): -DCMAKE_BUILD_TYPE=Release 
REM NOTE(Ryan): If you wish to change build type, must delete existing build/ folder.
cmake.exe -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"^
  -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld.exe" -DCMAKE_C_FLAGS="-target x86_64-windows-gnu" -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_C_COMPILER_ID="Clang"^
  -DVIDEO_OPENGL=ON -DDIRECTX=OFF -DRENDER_D3D=OFF -DVIDEO_DUMMY=OFF -DVIDEO_OPENGLES=OFF -DVIDEO_VULKAN=OFF^
  -DDISKAUDIO=OFF -DDUMMYAUDIO=OFF^
  -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_TEST=OFF ..
cmake.exe --build . -- -j %NUMBER_OF_PROCESSORS%

POPD

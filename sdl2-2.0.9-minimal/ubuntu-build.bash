#! /usr/bin/bash

set -e

[ ! -d "build" ] && mkdir build
pushd build 

# INFO(Ryan): -DCMAKE_BUILD_TYPE=Release 
# NOTE(Ryan): If you wish to change build type, must delete existing build/ folder.
cmake -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld" -DCMAKE_C_COMPILER="clang" -DCMAKE_C_COMPILER_ID="Clang" \
  -DPULSEAUDIO=ON -DDUMMYAUDIO=OFF -DDISKAUDIO=OFF -DOSS=OFF -DALSA=OFF -DJACK=OFF -DESD=OFF \
    -DARTS=OFF -DNAS=OFF -DSNDIO=OFF -DFUSIONSOUND=OFF -DLIBSAMPLERATE=OFF \
  -DVIDEO_X11=ON -DVIDEO_DUMMY=OFF -DVIDEO_WAYLAND_QT_TOUCH=OFF -DVIDEO_MIR=OFF \
  -DVIDEO_OPENGL=ON -DVIDEO_OPENGLES=OFF -DVIDEO_VULKAN=OFF -DVIDEO_DIRECTFB=OFF \
    -DVIDEO_VIVANTE=OFF -DVIDEO_KMSDRM=OFF -DVIDEO_RPI=OFF\
  -DPTHREADS=ON -DRPATH=OFF -DINPUT_TSLIB=OFF  -DSDL_DLOPEN=OFF \
  -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_TEST=OFF ..
cmake --build . -- -j $(getconf _NPROCESSORS_ONLN) 

popd
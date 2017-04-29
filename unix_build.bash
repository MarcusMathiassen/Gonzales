#!/usr/bin/env bash

output=-o\ app

wlvl=-Wall\ -Wno-unused-function
compiler=clang++
std=-std=c++1z
olvl=

src=./src/*.cpp
libs=-lglew\ -lglfw\ -framework\ OpenGL

start=`date +%s`
pushd ./build
make
popd
#$compiler $src $libs  $olvl $wlvl $output $std
end=`date +%s`

echo -e '\e[1;32m'Compilation finished. Time: $((end-start)) seconds

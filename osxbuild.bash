#!/usr/bin/env bash
if [ ! -d "./build" ]; then
  mkdir build > /dev/null 2>&1
fi
pushd ./build > /dev/null 2>&1
cmake ..
make
popd > /dev/null 2>&1

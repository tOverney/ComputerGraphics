#!/bin/sh -e
if [ ! -d  "build" ]; then
    mkdir "build"
fi

cd build
cmake ..
make -j 9
cd ../$1
./$1
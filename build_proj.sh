#!/bin/sh
if [ ! -d  "build" ]; then
    mkdir "build"
fi

cd build
cmake ..
make
cd $1
./$1
cd ../..
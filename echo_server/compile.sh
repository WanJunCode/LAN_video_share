#!/bin/bash

set -e

current=`date "+%Y-%m-%d %H:%M:%S"`
timeStamp=`date -d "$current" +%s`

rm -rf bin
mkdir bin

rm -rf build
mkdir build

cd build
cmake ..
make -j3
make install
cd -

# cd bin
# eu-strip echo_server -f echo_server.debug
# cd -

echo "$current Success to compile"

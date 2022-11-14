#!/bin/bash

set -e

current=`date "+%Y-%m-%d %H:%M:%S"`
timeStamp=`date -d "$current" +%s`

# 保证所需要的第三方库全部下载
cd conan_project/build/
conan install ..
cd -

rm -rf bin/*
rm -rf build/*
rm -rf lib/*

cd build
cmake ..
make -j3
make install
make CopyTask
cd -

# cd bin
# eu-strip echo_server -f echo_server.debug
# cd -

echo "$current Success to compile"

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
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/code/LAN_video_share/lan_video_server/lib
# ./lan_video_test
# cd -

# cd bin
# eu-strip echo_server -f echo_server.debug
# cd -

echo "$current Success to compile"

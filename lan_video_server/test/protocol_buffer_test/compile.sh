#!/bin/bash

rm -rf generated/include/*
rm -rf generated/src/*

cd config
/root/.conan/data/protobuf/3.17.1/_/_/package/1318f50b2be671a0caff945a0bd4dea23311e607/bin/protoc *.proto --cpp_out=../build/
# protoc *.proto --cpp_out=../build/
cd -

cd build/
mv *.cc ../generated/src/
mv *.h ../generated/include/
cd -

# protoc --proto_path=. --cpp_out=. my.proto
# g++ generated/src/*.cc src/reader.cpp -o read_demo -I generated/include/ -lprotobuf -std=c++11 -lpthread
# g++ generated/src/*.cc src/writer.cpp -o writer_demo -I generated/include/ -lprotobuf -std=c++11 -lpthread

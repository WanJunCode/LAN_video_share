#!/bin/bash

rm -rf generated/include/*
rm -rf generated/src/*

cd config
protoc *.proto --cpp_out=../build/
cd -

cd generated
mv *.cc src/
mv *.h include/
cd -

# protoc --proto_path=. --cpp_out=. my.proto
# g++ my.pb.cc reader.cpp -o read -lprotobuf -std=c++11 -lpthread
# g++ my.pb.cc writer.cpp -o writer -lprotobuf -std=c++11 -lpthread
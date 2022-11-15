#!/bin/bash
rm -rf src/*
rm -rf tmp/*
./script/fastddsgen idl/* -d src/
./script/fastddsgen -example CMake idl/*.idl -d tmp/
#! /bin/bash

cmake -S . -B build -D CMAKE_BUILD_TYPE=debug
cmake --build . --parallel

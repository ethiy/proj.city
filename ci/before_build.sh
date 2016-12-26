#! /usr/bin/env bash

mkdir build && cd build
cmake -DCGAL_DONT_OVERRIDE_CMAKE_FLAGS=ON ..

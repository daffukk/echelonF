#!/bin/bash

set -e 

mkdir -p build
cd build

cmake ..
make

mv -f EServer EClient ..
echo "Compilation completed."

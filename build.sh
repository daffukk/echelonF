#!/bin/bash

set -e 

mkdir -p build
cd build

cmake ..
make

mv -f ecf ..
echo "Compilation completed."

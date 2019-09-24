#!/bin/bash
mkdir -p tests
cp data/*.* tests/
cd tests
cmake .
make
chmod +x runTests
./runTests
cd ..
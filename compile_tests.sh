#!/bin/bash
cd tests
cmake .
make
chmod +x runTests
cd ..
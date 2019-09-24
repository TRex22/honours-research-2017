#!/bin/bash
make gtest
make main
make test
chmod +x tests/runTests
# bash compile_tests.sh
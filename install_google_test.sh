#!/bin/bash
sudo apt-get install libgtest-dev
sudo mkdir -p /usr/src/
sudo cp -rf ./include/googletest/gtest /usr/src
# googletest
# sudo apt-get install cmake # install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib

rm -rf ~/Research_Honours
make clean
clear
clear
# make
cp -rf ../Research_Honours ~/
# make -C ~/Research_Honours

CURRENT_DIR = pwd
cd ~/Research_Honours
bash ~/Research_Honours/build_all.sh
bash ~/Research_Honours/compile_tests.sh
~/Research_Honours/tests/runTests
# ~/Research_Honours/bin/game.out --run_tests
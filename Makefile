#g++ -fopenmp -std=c++11 src/main.cpp -o bin/game.out -Wall -Wextra -I "include/rapidjson" -I "include/googletest" #-O3

GTEST_DIR = "include/googletest"
RAPIDJSON_DIR = "include/rapidjson"
SDL_DIR = "/usr/include/SDL2"
TEST_DIR = tests 

CXX = g++
# -lSDL2main -lSDL2_image -mrdseed
CPPFLAGS += -fopenmp -std=c++11
#-isystem $(GTEST_DIR)/include     -I $(SDL_DIR) 
CXXFLAGS += -g -Wall -Wextra -pthread -I $(RAPIDJSON_DIR) -lSDL2 -lSDL2_image -lSDL2_ttf -O3 
#-Ofast#-O3

MAIN = main.cpp
MAIN_OUT = game
TESTS = rules_uitest

all: 
	make clean
	make main
	# make test
	make sdl

test:
	# $(CXX) $(CPPFLAGS) -lgtest_main ./$(TEST_DIR)/$(TESTS).cpp -o ./$(TEST_DIR)/$(TESTS).o
	bash compile_and_run_tests.sh

main:
	mkdir -p bin
	mkdir -p results
	$(CXX) $(CPPFLAGS) ./src/$(MAIN) -o bin/$(MAIN_OUT).out $(CXXFLAGS)
	cp -rf data/* bin/

main_nomp:
	mkdir -p bin
	mkdir -p results
	$(CXX) -std=c++11 ./src/$(MAIN) -o bin/$(MAIN_OUT).out $(CXXFLAGS)
	cp -rf data/* bin/

sdl:
	mkdir -p bin
	$(CXX) $(CPPFLAGS) ./src/sdl_main.cpp -o bin/demo.out $(CXXFLAGS)
	cp -rf data/* bin/

clean :
	rm -rf bin/images
	rm -f bin/*

	rm -f tests/*.o
	rm -f tests/runTests 
	rm -f tests/cmake_install*
	rm -f tests/CMakeCache*
	rm -rf tests/CMakeFiles
	rm -f tests/*.json

gtest:
	bash install_google_test.sh


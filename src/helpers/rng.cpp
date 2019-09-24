// #include "immintrin.h"
// #include <cstdint>

// class TrueRNDIntrinstic {
// public:
//   typedef unsigned int result_type;
//   static unsigned int min() { return 0; }
//   static unsigned int max() { return UINT_MAX; }
//   unsigned int operator()() {
//     // generate a random number in the range [0, 42]
//   	//uint64_t 
//     unsigned int val = 0;
// 	if(!_rdrand32_step(&val)) {
// 	  printf("Error generating hardware random value\n");
// 	}

// 	// long unsigned int luival;
// 	return val;
//   }
// };

// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
// Marsaglia's xorshf generator
class MarsagliaXorshfRNG {
public:
  typedef unsigned int result_type;
  static unsigned int min() { return 0; }
  static unsigned int max() { return UINT_MAX; }
  unsigned int operator()() {
    // generate a random number in the range [0, 42]
    static unsigned long x=123456789, y=362436069, z=521288629;
    unsigned long t;
    unsigned int out = 0;

	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	out = (unsigned int)z;
	return out;
  }
};


class FastRandRNG {
public:
  typedef unsigned int result_type;
  static unsigned int min() { return 0; }
  static unsigned int max() { return UINT_MAX; }
  unsigned int operator()() {
    // generate a random number in the range [0, 42]
    std::random_device d;
    unsigned int g_seed = d();
    g_seed = (214013*g_seed+2531011); 
  	g_seed = (g_seed>>16)&0x7FFF; 

	return g_seed;
  }
};


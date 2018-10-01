
/*
This program benchmarks the access time for one access on a buffer
by doing many repetitions. Based on our results, we recommend doing
this many times and taking the median access time, as it is the most
representative of the access time without long interrupts.

By Marika Swanberg and Jillian James
*/
#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <assert.h>

// This function makes a buffer of size "size" with random
// entries to trick the compiler.
int* make_buffer(int size){
	// The buffer is deleted in main before exiting.
	int* buffer = new int[size];
	for(int i = 0; i < size; i++){
 		buffer[i] = (int)rand();
	}
	return buffer;
}


// This times iters number of accesses to the buffer and returns 
// the average access time in nanoseconds. To trick the compiler, we
// use a hash function to access elements out of order.
double hash_access(int* buffer, const int64_t iters, const int size){

	int hashed;
	int x = 0;


	auto start = std::chrono::steady_clock::now();
	for(int64_t i = 0; i < iters; i++){
		// We are accessing hashed locations in the buffer to ensure that
		// the cache won't prefetch values.
		hashed = ((i*i)%(size+i) + i)%(size-1);
		// We need to access the value.
		x = buffer[hashed];
	}
	auto end = std::chrono::steady_clock::now();
	auto diff = (end - start);

	//Get time difference in nanoseconds
	auto time_span = std::chrono::duration <double, std::nano> (diff).count();
	time_span = time_span/iters;

	std::cout << std::to_string(size) << ", " << std::to_string(time_span) << std::endl;
	// We needed to use x in order to trick the compiler.
	return x;
}





int main(int argc, char* argv[]){
    if(argc != 3){
        assert(0 && "Not enough arguments");
	  	exit(-1);
    }

	srand(time(NULL)); // set the seed

	
	const int size = atoi(argv[1]);
	const int iters = atoi(argv[2]);
	int* buffer =  make_buffer(size);
	hash_access(buffer, iters, size);

    // Delete the dynamically-allocated buffer
	delete(buffer);
	return 0;

	}









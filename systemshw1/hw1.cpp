
/*
CSCI 389
Jillian James and Marika Swanberg
HW 1

This program takes two arguments "size" and "iters". It creates a buffer of length "size" and times how long it takes to perform
"iters" number of accesses to the buffer. In order to measure the latency of memory accesses we needed to foil the compiler so it
wouldn't prefetch the array values. To do this, we made a hash function to probe the buffer and access the values stored in memory.

To run navigate to the directory containing hw1.cpp and run:
	g++ -std=c++11 -O3 -Werror -Wall -Wextra -pedantic -o  buffer hw1.cpp
	./buffer (size) (iters)
where (size) and (iters) are the desired arguments.



This program was run with:

OS: Ubuntu 17.10
Memory: 3.9 GiB
Processor: Intel Core i5-7267U CPU @3.10GHz
OS type: 64-bit
Disk 66.3 GB

*/
#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <assert.h>

// This function makes a buffer of size "size". It contains
// "size" number of random ints between 0 and RAND_MAX
uint8_t* make_buffer(int size){
	uint8_t* buffer = new uint8_t[size];	//throws a bad alloc acception if fails.
	for(int i = 0; i < size; i++){
 		buffer[i] = (uint8_t)(rand()%127);
	}
	return buffer;
}


// This timing function times iters number of accesses to the buffer and returns 
// the average access time in nanoseconds. To trick the compiler, we
// use a hash function to access elements out of order.
uint8_t hash_access(uint8_t* buffer, const int64_t iters, const int size){

	int hashed;
	uint8_t x = 0;


	auto start = std::chrono::steady_clock::now(); //Start clock.

	for(int64_t i = 0; i < iters; i++){
		// We are accessing hashed locations in the buffer to ensure that
		// the cache won't prefetch values.
		hashed = ((i*i)%(size+i) + i)%(size-1);
		// We need to access the value.
		x = buffer[hashed];
	}

	auto end = std::chrono::steady_clock::now(); //End clock.

	//Get time difference in nanoseconds.
	auto diff = (end - start);
	auto time_span = std::chrono::duration <double, std::nano> (diff).count();
	
	//Get average access time.
	time_span = time_span/iters;

	//Output: size, time_span for the csv files.
	std::cout << std::to_string(size) << ", " << std::to_string(time_span) << std::endl;

	//We need to do something with x to ensure the compiler bothered to do line 38.
	return x;
}

// void printarray(uint8_t* array, const int size) {
// 	for(int i = 0; i <size; i++){
// 		uint8_t value = array[i];
// 		std::cout << std::to_string(value) << ", ";
// 	}
// }



int main(int argc, char* argv[]){
    if(argc != 3){
        assert(0 && "Not enough arguments");
	  	exit(-1);
    }

	srand(time(NULL)); // set the seed

	
	const int size = atoi(argv[1]);
	const int iters = atoi(argv[2]);


	//Make a buffer (an array) of length "size" and fill it using rand()
	uint8_t* buffer =  make_buffer(size);
	//printarray(buffer, size);

	//Access the buffer in an unpredictable manner and measure the latency
	hash_access(buffer, iters, size);

    	//Free up memory.
    	delete[] buffer;


	return 0;

	}









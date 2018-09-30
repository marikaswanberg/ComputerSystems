
/*
put a description here

*/
#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <assert.h>

// This function makes a buffer of size "size."
int* make_buffer(int size){
	int* buffer = new int[size];
	for(int i = 0; i < size; i++){
 		buffer[i] = rand();
	}
	return buffer;
}

// Our hash function
int hash(int i, int m){
  return ((i*i)%(m+i) + i)%m;
}

// This times iters number of random accesses to the buffer and returns 
// the average access time in nanoseconds. Note length of rand_locations 
// should be equal to iters.
double random_access(int* buffer, const int iters, const int size){
	int x;
	int hashed;
	auto start = std::chrono::steady_clock::now();
	for(int i = 0; i < iters; i++){
		// We are accessing random locations in the buffer to ensure that
		// the cache won't prefetch values.
	    hashed =  ((i*i)%(size+i) + i)%size;
		x = buffer[hashed];
		x+=1;
	}
	auto end = std::chrono::steady_clock::now();

 // we needed to 'use' the variable x
	//std::chrono::duration <double,std::nano> time_span = std::chrono::duration <double, std::nano> (end-start).count();
	// don't forget to take the average

	std::cout << "Time_span:" << std::chrono::duration <double, std::nano> (end-start).count() << "\n";
	//return time_span/iters
	return 0.0;
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

	
	double time = random_access(buffer, iters, size);
	//std::cout << "Accesses to buffer size " << std::to_string(size) + " took " << std::to_string(time) << " nanoseconds"<< std::endl;
    
    std::cout << std::to_string(size) << ", " << std::to_string(time) << std::endl;

	delete(buffer);
	return 0;

	}









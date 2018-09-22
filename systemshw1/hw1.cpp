
/*
put a description here

*/
#include <random>
#include <fstream>
#include <iostream>
#include <chrono>

// This function makes a buffer of size "size."
int* make_buffer(int size){
	int* buffer = new int[size];
	for(int i = 0; i < size; i++){
		buffer[i] = i;
	}
	return buffer;
}

// This returns an array of size iters of random integers that will be the access locations
// in the buffer.
int* make_rand_array(int size, const int iters){
	int* rand_array = new int [iters]; // dynamically allocate our random accesses
	for(int i = 0; i < iters; i++){
		// Note that rand() has a maximum value of ~33,0000, so we expand
		// rand() to a range that covers the full length of the buffer
		rand_array[i] = size * (int)(rand()/ RAND_MAX+1);
	}
	return rand_array;
}

// This times iters number of random accesses to the buffer and returns 
// the average access time in nanoseconds. Note length of rand_locations 
// should be equal to iters.
double random_access(int* buffer, int* rand_locations, const int iters){
	int x;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for(int i = 0; i < iters; i++){
		// We are accessing random locations in the buffer to ensure that
		// the cache won't prefetch values.
		x = buffer[rand_locations[i]];
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	x+=1; // we needed to 'use' the variable x

	// don't forget to take the average
	return std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(); 	//convert to nanoseconds
}





int main(int argc, char* argv[]){
        if(argc < 3){
                // not enough arguments
	  exit(-1);
        }

	srand(time(NULL)); // set the seed

	
	const int size = atoi(argv[1]);
	const int iters = atoi(argv[2]);


	int* buffer;
	int* rand_locations;

	buffer = make_buffer(size);
	rand_locations = make_rand_array(size, iters);
	double time = random_access(buffer, rand_locations, iters);
	std::cout << std::to_string(size) + ", " << std::to_string(time) << std::endl;
	return 0;

	}









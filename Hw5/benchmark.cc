
#include <chrono>
#include <string>
#include <iostream>
#include <unistd.h>
#include "cache.hh"
#include <vector>
#include <cassert>


void fill_cache(Cache &mycache, uint64_t num_items) {
	std::string val;
	for(uint64_t i = 0; i < num_items; i++){
		val = std::to_string(i);
		mycache.set(val, &val, sizeof(std::string));
	}
}

std::vector<int> make_rand_operation_vect(uint64_t requests_per_sec){
	std::vector<int> random_operation;
	for (unsigned int i = 0; i < requests_per_sec*10; i++){
		int randnum = (rand() % 100 + 1);
		random_operation.push_back(randnum);
	}
	return random_operation;
}

std::vector<std::string> make_rand_key_vect(uint64_t requests_per_sec, uint64_t num_items){
	std::vector<std::string> random_keys;
	for (unsigned int i = 0; i < requests_per_sec*10; i++){
		std::string randkey = std::to_string((rand() % num_items));
		random_keys.push_back(randkey);
	}
	return random_keys;
}


double throughput_benchmark(uint64_t requests_per_sec, uint64_t num_items, Cache &mycache, uint64_t &actual_requests_per_sec) {
	uint64_t micro_per_request = ((1.0/(requests_per_sec))*1000000); //microseconds per request
	// make randomness
	std::vector<int> operation_vector = make_rand_operation_vect(requests_per_sec);
	std::vector<std::string> key_vector = make_rand_key_vect(requests_per_sec, num_items);

	uint64_t total_response_time = 0;

	int response = 0; //fool the compiler
	auto t1 = std::chrono::steady_clock::now(); //outer timer t1
	for(uint64_t i = 0; i < (requests_per_sec*10); i++){
		auto it1 = std::chrono::steady_clock::now();

		int randnum = operation_vector.at(i);
		std::string randkey = key_vector.at(i);

		if (randnum > 4) {
			Cache::index_type size = 0;
			mycache.get(randkey, size);
		} else if (randnum == 4) {
			response = mycache.del(randkey);
		} else {
			response = mycache.set(randkey, &randkey, sizeof(randkey));
		}
		auto it2 = std::chrono::steady_clock::now(); //inner timer it2

		auto response_time = std::chrono::duration <double, std::micro> (it2 - it1).count();
		total_response_time += response_time;
		double time_left = (micro_per_request - response_time);
		//std::cout << micro_per_request << ", response_time: "<< response_time << ", time left: " << time_left << std::endl;
		if (time_left > 0) {
			usleep(time_left);
		}
	}
	assert(response != 3);
	auto t2 = std::chrono::steady_clock::now();//outer timer t2
	auto total_time = std::chrono::duration <double, std::micro> (t2 - t1).count();

	actual_requests_per_sec = (requests_per_sec*10.0)/(total_time/1000000.0);

	double average_response_time = total_response_time/(requests_per_sec*10.0);
	return average_response_time; // average time in microseconds
}


int main( int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Requires arguments: num_items, requests_per_sec" <<std::endl;
		return 1;
	}

	uint64_t num_items = std::atoi(argv[1]);
	uint64_t reqs_per_sec = std::atoi(argv[2]);

	uint64_t actual_requests_per_sec = 0;

	Cache mycache((num_items*sizeof(std::string))+100); //adds an extra hundred so some sets don't require evictions
	fill_cache(mycache, num_items);
	double average_response_time = throughput_benchmark(reqs_per_sec, num_items, mycache, actual_requests_per_sec);
	double milli_response_time = average_response_time/1000;
	std::cout << num_items << ", " << milli_response_time << std::endl;


return 0;
}



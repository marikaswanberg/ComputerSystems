#include "pistache/include/pistache/endpoint.h"
#include "cache.hh"

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>


// #include "crow.h"


std::vector<std::string> parse_request(std::string request) {
	std::vector<std::string> request_vector;
	size_t current_index = 0;
	size_t prev_index = 0;
	while(current_index != std::string::npos) {
		current_index = request.find("/", current_index);
		request_vector.push_back(request.substr(prev_index, current_index-prev_index));
		prev_index = current_index+1;
		if (current_index != std::string::npos) {
			current_index++;
		}
	}
	request_vector[0] = request_vector[0].substr(0, request_vector[0].find(" "));

	return request_vector;
}


int main( int argc, char* argv[])
{

   	Cache::index_type maxmem = 20; 
    int portnum = 3724; //join our igloo ;)
    
    // parse the maxmem and portnum inputs
    int c ;
    while( ( c = getopt (argc, argv, "m:t:") ) != -1 ) 
    {
        switch(c)
        {
            case 'm':
                if(optarg) maxmem = (Cache::index_type)std::atoi(optarg);
                break;
            case 't':
                if(optarg) portnum = std::atoi(optarg) ;
                break;
        }
    }



}
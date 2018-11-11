#include "pistache/include/pistache/endpoint.h"
#include "cache.hh"

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>


#include "crow.h"


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


	Cache server_cache(maxmem);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/memsize").methods("GET"_method)

    CROW_ROUTE(app, std::string request)([](){
        std::vector<std::string> request_vector = parse_request(request);

        if ((request_vector[0] == "GET") && (request_vector[1] == "memsize")) {
        	mem = server_cache.space_used();
        	crow::json::wvalue x;
        	x["memused"] = mem;
        	return x;
        	// send a JSON thing
        }
        if ((request_vector[0] == "GET") && (request_vector[1] == "key")) {
        	val = server_cache.get(request_vector[2]);
        	crow::json::wvalue x;
    		x["key"] = request_vector[2];
    		x["value"] = val;
    		return x;
        	// JSON here
        }

       	if ((request_vector[0] == "PUT") && (request_vector[1] == "key")){
       		server_cache.set(request_vector[2], &request_vector[3], sizeof(request_vector[3]));

 			
       	}
       	if ((request_vector[0] == "DELETE") && (request_vector[1] == "key")){
       		server_cache.del(server_cache[2]);
       	}
       	if (request_vector[0] == "HEAD" && (request_vector[1] == "key")){

       		// idk
       	}
       	if (request_vector[0] == "POST" && (request_vector[1] == "shutdown")){
       		// shutdown
       	}

       	else{
        	//raise an error
        	assert(0 && "Not a valid request");
        }
 
    });

    app.port(18080).run();




}
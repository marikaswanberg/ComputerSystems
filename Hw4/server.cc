#include "pistache/endpoint.h"
#include "cache.hh"

#include <iostream>
#include <unistd.h>
#include <cstdlib>



using namespace Net;

class CacheHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(CacheHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
         response.send(Http::Code::Ok, "Hello, World");
    }
};

int main( int argc, char* argv[] )
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
      std::cout << "maxmem: "<< maxmem <<" portnum: " << portnum<< std::endl ;   


 	Cache mycache(maxmem);

    Net::Address addr(Net::Ipv4::any(), Net::Port(portnum));

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();


}
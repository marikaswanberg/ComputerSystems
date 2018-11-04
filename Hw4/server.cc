#include "pistache/endpoint.h"
#include "cache.hh"

#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main( int argc, char* argv[] )
{
   	Cache::index_type maxmem = 20; 
    int portnum = 2033 ;
    
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
}

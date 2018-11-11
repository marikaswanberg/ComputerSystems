#include <string>
#include "cache.hh"

class Server{
    int portnum_;
    int new_socket_;
public:
    Server(int portnum);
    ~Server();
    void start_listen();
    void read_and_parse(Cache server_cache);
};
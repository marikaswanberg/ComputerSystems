#include <string>
#include <vector>
#include "cache.hh"

class Server{
    int portnum_;
    int new_socket_;
public:
	bool running_;
    Server(int portnum);
    ~Server();
    void start_listen();
    std::vector<std::string> parse_request(std::string request);
    std::string create_response(std::vector<std::string> request_vector, Cache &server_cache);
    void read_and_parse(Cache &server_cache);


};
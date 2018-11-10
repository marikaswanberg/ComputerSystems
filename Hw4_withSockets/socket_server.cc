#include "server.hh"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>

Server::Server(int portnum=8080){
    portnum_ = portnum;
}

Server::~Server(){

}
void Server::start_listen(){
    int server_fd; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);   

    std::cout << "starting..." << std::endl;
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        std::cout << "socket failed" << std::endl;
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        std::cout << "set socket opt" << std::endl;
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( portnum_ ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        std::cout << "bind failed" <<std::endl;
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        std::cout << "listen failed" << std::endl;
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket_ = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        std::cout << "accept failed" << std::endl;
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    return;
}
void Server::read_and_parse(Cache server_cache){
    char buffer[1024] = {0};
    int valread = read(new_socket_ , buffer, 1024); 
    std::vector<std::string> parsed_request = parse_request(buffer);
    printf("%s\n",buffer ); 
    char hello[] = "Hello from server";
    send(new_socket_ , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
}

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

    // response??
    if(request_vector[0] == "GET"){
        if (request_vector[1] == "key"){
            server_cache.get(request_vector[2]);
        }
        if (request_vector[1] == "memsize"){
            response = server_cache.memused();
        }
        else{
            // error
        }
    }
    if((request_vector[0] == "PUT") && (request_vector[1] == "key")){
        // set new value or update old one
        server_cache.set(request_vector[2], &(request_vector[3]));
    }
    if((request_vector[0] == "POST") && request_vector[1] == "shutdown"){
        //shutdown somehow
    }
    if((request_vector[0] == "DELETE")&& (request_vector[1] == "key")){
        server_cache.del(request_vector[2]);
    }
    if(request_vector[0] == "HEAD"){
        //idk how to return just the header
    }
    else {
        // raise an error for invalid message/request
    }
}



int main() 
{
    Cache mycache(10);
    Server myserver;
    std::cout << "a" << std::endl;
    myserver.start_listen();
    std::cout << "b" << std::endl;
    myserver.read_and_parse(mycache);
    std::cout << "c" << std::endl;
    return 0; 
}
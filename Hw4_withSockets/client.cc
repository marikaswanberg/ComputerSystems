#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <vector>
#include <arpa/inet.h>

#include "cache.hh"
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <tuple>


struct Cache::Impl {
	index_type maxmem_;
	hash_func hasher_;
	index_type memused_;
	index_type newest_;
	std::unordered_map<std::string, std::tuple<val_type, index_type, index_type>> unorderedmap_;
	const int portnum_ ;
	int sock_;

public:
	Impl(index_type maxmem, hash_func hasher) 
	: maxmem_(maxmem), hasher_(hasher), memused_(0), newest_(0), unorderedmap_(), portnum_(8080), sock_(0){
		//struct sockaddr_in address;  
	    struct sockaddr_in serv_addr;  
	    //char buffer[1024] = {0}; 
	    if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	    { 
	        printf("\n Socket creation error \n");  
	    } 
	   
	    memset(&serv_addr, '0', sizeof(serv_addr)); 
	   
	    serv_addr.sin_family = AF_INET; 
	    serv_addr.sin_port = htons(portnum_); 
	       
	    // Convert IPv4 and IPv6 addresses from text to binary form 
	    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
	    { 
	        printf("\nInvalid address/ Address not supported \n");
	    } 
	   
	    if (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	    { 
	        printf("\nConnection Failed \n"); 
	    } 
	}

	~Impl(){
		// shutdown the server
		std::string shutdown_msg = "POST /shutdown";
		send(sock_, shutdown_msg.c_str(), shutdown_msg.size()+1, 0);

		}

	int set(key_type key, val_type val, index_type size){
		const std::string* value = static_cast<const std::string*>(val);
		std::string message = "PUT /key/" + key + "/" + value->c_str();
		char buffer[1024] = {0};
		size ++; // we need to use size, but don't actually need it
		send(sock_ , message.c_str() , message.size()+1 , 0 );  
    	read(sock_ , buffer, 1024);
		return std::atoi(buffer);
	}

	val_type get(key_type key, index_type& val_size) const {
		char buffer[1024] = {0};
		std::string message = "GET /key/" + key;
		send(sock_, message.c_str(), message.size()+1, 0);
		read(sock_ , buffer, 1024);
		std::string strbuffer = static_cast<std::string>(buffer);
		size_t value_index = strbuffer.find("value: ", 0) + 7;
		std::string value = strbuffer.substr(value_index, (strbuffer.length() - value_index -1));
		if (value == "nullptr"){
			val_size = 0;
			return nullptr;
		}
		val_size = sizeof(value);
		val_type val = &value;
		return val;
	}
	
	// This deletes a (key, tuple) entry from the map
	int del(key_type key){
		char buffer[1024] = {0};
		std::string message = "DELETE /key/" + key;
		send(sock_, message.c_str(), message.size()+1, 0);
		read(sock_ , buffer, 1024);
		std::string strbuffer = static_cast<std::string>(buffer);
		return std::stoi(strbuffer);
	}

	index_type space_used() const{
		
		char buffer[1024] = {0};
		std::string message = "GET /memsize";
		send(sock_, message.c_str(), message.size()+1, 0);
		read(sock_ , buffer, 1024);
		std::string strbuffer = static_cast<std::string>(buffer);
		size_t value_index = strbuffer.find("memused: ", 0) + 9;
		std::string mem = strbuffer.substr(value_index, (strbuffer.length() - value_index -1));
		return std::stoi(mem);
	}

};

Cache::Cache(index_type maxmem, hash_func hasher) 
	: pImpl_(new Impl(maxmem,hasher))
 {

}

Cache::~Cache(){
	// we don't need anything here because pimpl is a unique_ptr
}

// Add a <key, value> pair to the cache.
// If key already exists, it will overwrite the old value.
// Both the key and the value are to be deep-copied (not just pointer copied).
// If maxmem capacity is exceeded, sufficient values will be removed
// from the cache to accomodate the new value.
int Cache::set(key_type key, val_type val, index_type size){
	return pImpl_->set(key,val,size);
}

// Retrieve a pointer to the value associated with key in the cache,
// or NULL if not found.
// Sets the actual size of the returned value (in bytes) in val_size
Cache::val_type Cache::get(key_type key, index_type& val_size) const {
	return pImpl_->get(key,val_size);
}

// Delete an object from the cache, if it's still there
int Cache::del(key_type key){
	return pImpl_->del(key);
}

// Compute the total amount of memory used up by all cache values (not keys)
Cache::index_type Cache::space_used() const {
	return pImpl_->space_used();
}

// int main(){
// // std::cout << "test_init_empty" << std::endl;
//  //  	Cache mycache(10);
//  //  	std::cout << (mycache.space_used() == 0) << std::endl;

// 	std::cout << "test_inc_memused1" << std::endl;
// 	Cache mycache2(100);
//   	std::string value = "marika";
//     mycache2.set("hello", &value, sizeof(value));
//     std::cout << "passed set, starting spaceused" << std::endl;
//     Cache::index_type size = mycache2.space_used();
//     std::cout << "finished space_used" << std::endl;
//     std::cout << (size == sizeof(value)) << std::endl;
//     return 0;
// }






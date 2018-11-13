// create_connection(){
    
// }

// send_request(){}

// read_response(){}

// main{
    
//     create connection
//     send request
//     read response
// }


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
	const int portnum_ ;
	int sock_; 
	std::unordered_map<std::string, std::tuple<val_type, index_type, index_type>> unorderedmap_;
public:
	Impl(index_type maxmem, hash_func hasher) 
	: maxmem_(maxmem), hasher_(hasher), memused_(0), newest_(0), unorderedmap_(), portnum_(8080), sock_(0){
		struct sockaddr_in address;  
	    struct sockaddr_in serv_addr;  
	    char buffer[1024] = {0}; 
	    if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	    { 
	        printf("\n Socket creation error \n"); 
	        return -1; 
	    } 
	   
	    memset(&serv_addr, '0', sizeof(serv_addr)); 
	   
	    serv_addr.sin_family = AF_INET; 
	    serv_addr.sin_port = htons(portnum_); 
	       
	    // Convert IPv4 and IPv6 addresses from text to binary form 
	    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
	    { 
	        printf("\nInvalid address/ Address not supported \n"); 
	        return -1; 
	    } 
	   
	    if (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	    { 
	        printf("\nConnection Failed \n"); 
	        return -1; 
	    } 
	}

	~Impl(){
		// idk how this is going to work
		auto it = unorderedmap_.begin();
		while (it != unorderedmap_.end()) {
			delete[] static_cast<const char*>(std::get<0>(it->second));
			it = unorderedmap_.erase(it);
		}
	}

	int set(key_type key, val_type val, index_type size){
		std::string message = "POST key/" + key + "/" + val;
		send(sock_ , message , sizeof(message) , 0 );  
    	read( sock , buffer, 1024);
		return std::atoi(buffer);
	}

	val_type get(key_type key, index_type& val_size) const {
		
		return std::get<0>(entry);
	}
	
	// This deletes a (key, tuple) entry from the map
	int del(key_type key){
		
		return 0;
	}

	index_type space_used() const{
		
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

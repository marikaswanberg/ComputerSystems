/*
Jillian James and Marika Swanberg
This is the implementation file for cache.hh called cache.cc

Run with the command:
g++ -Wall -Wextra -Werror -pedantic cache.cc test.cc -o test

On a machine using:
OS: Ubuntu 17.10
Memory: 3.9 GiB
Processor: Intel Core i5-7267U CPU @3.10GHz
OS Type: 64-bit
Disk 66.3 GB
*/




#include "cache.hh"
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <tuple>


struct Cache::Impl {
	index_type maxmem_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type memused_;
	index_type newest_;
	std::unordered_map<std::string, std::tuple<val_type, index_type, index_type>> unorderedmap_;
public:
	Impl(index_type maxmem, evictor_type evictor, hash_func hasher) 
	: maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0), newest_(0), unorderedmap_(){

	}
	~Impl(){
		//unorderedmap_.clear();
	}


	void set(key_type key, val_type val, index_type size){
		val_type copy = new val_type[size];
		std::memcpy((void*)copy, val, size);
		std::tuple<val_type, index_type, index_type> entry = std::make_tuple((void*)copy, size, newest_);
		if ((memused_ + size) > maxmem_) {
			evictor();
		}
		this->unorderedmap_[key] = entry;
		memused_+= size;
		newest_ += 1;
		return;
	}

	val_type get(key_type key, index_type& val_size) const {
		std::tuple<val_type, index_type, index_type> entry;
		try{
			entry = unorderedmap_.at(key);
		} catch(const std::out_of_range& oor) {
			val_size = 0; 	// set val_size to 0 if we don't find the value
			return NULL;
		}
		
		val_size = std::get<1>(entry);
		return std::get<0>(entry);
	}
	

	void del(key_type key){
		index_type val_size = 0;
		val_type val = get(key, val_size);
		unorderedmap_.erase(key);
		free((void*)val);
		memused_ -= val_size;
		return;
	}

	index_type space_used() const{
		return memused_;

	}


	void evictor() {
		std::string oldest_key;
		index_type oldest_age = newest_;
		for( auto& pair: unorderedmap_){
			std::tuple<val_type, index_type, index_type> entry = unorderedmap_.at(pair.first); // entry = (value, age) tuple
			index_type age =  std::get<2>(entry); // get the age
			if (age <= oldest_age){
				oldest_age = age;
				oldest_key = pair.first;
			}
	}
	del(oldest_key);
	
}

	void inc_newest(){
		newest_+= 1;

	}

};

Cache::Cache(index_type maxmem,
        evictor_type evictor,
        hash_func hasher) 
	: pImpl_(new Impl(maxmem,evictor,hasher))
 {

}

Cache::~Cache(){
	// we don't need anything here because of pimpl?
}

void Cache::set(key_type key, val_type val, index_type size){
	//set the key/value etc
	return pImpl_->set(key,val,size);
}

Cache::val_type Cache::get(key_type key, index_type& val_size) const {
	//get the key/values
	return pImpl_->get(key,val_size);
}

void Cache::del(key_type key){
	// delete a key/value pair
	pImpl_->del(key);
}

Cache::index_type Cache::space_used() const {
	return pImpl_->space_used();
}

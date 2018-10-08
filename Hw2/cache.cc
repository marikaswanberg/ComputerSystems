#include "cache.hh"
#include <unordered_map>

struct Cache::Impl {
	index_type maxmem_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type memused_;
	std::unordered_map<key_type, val_type> map;

public:
	Impl(index_type maxmem, evictor_type evictor, hash_func hasher) 
	: maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0){
		map = std::unordered_map<key_type, val_type>();
	}


	void set(key_type key, val_type val, index_type size){
	//set the key/value etc
	}

	val_type get(key_type key, index_type& val_size) const {
	//get the key/value
	}

	void del(key_type key){
		// delete a key/value pair
	}

	index_type space_used() const{

	}
};

Cache::Cache(index_type maxmem,
        evictor_type evictor,
        hash_func hasher) 
	: pImpl_(new Impl(maxmem,evictor,hasher))
 {
 // 	pimpl_ -> std::unordered_map<int, int> cachemap;
	// myMaxmem = maxmem;
	// myEvictor = evictor;
	// myHasher = hasher;
	// // put our stuff here
}

Cache::~Cache(){
	// we don't need anything here because of pimpl?
}

void Cache::set(key_type key, val_type val, index_type size){
	//set the key/value etc
	return pImpl_->set(key,val,size);
}

Cache::val_type Cache::get(key_type key, index_type& val_size) const {
	//get the key/value
	return pImpl_->get(key,val_size);
}

void Cache::del(key_type key){
	// delete a key/value pair
	pImpl_->del(key);
}

Cache::index_type Cache::space_used() const {
	return pImpl_->space_used();
}







// class foo::Impl {
//  public:
//   int value_;
// };

// foo::foo()
//  : pimpl_(new Impl())
// {
//   pimpl_->value_ = 56;
// }


// foo::~foo()
// {
// }

// int foo::dosomething()
// {
//   return pimpl_->value_;
// }

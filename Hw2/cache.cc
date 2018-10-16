#include "cache.hh"
#include <cstring>
#include <unordered_map>

struct Cache::Impl {
	index_type maxmem_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type memused_;
	std::unordered_map<std::string, void*> unorderedmap_;

public:
	Impl(index_type maxmem, evictor_type evictor, hash_func hasher) 
	: maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0), unorderedmap_(){

	}


	void set(key_type key, val_type val, index_type size){
		val_type copy = new val_type[size];
		std::memcpy((void*)copy, val, size);
		if ((memused_ + size) > maxmem_) {
			//evict
		}
		this->unorderedmap_[key] = (void*)copy;
		memused_+=size;
		return;
	}

	val_type get(key_type key, index_type& val_size) const {
	//get the key/value
		for( auto const& pair: unorderedmap_){
			if (key == pair.first) {
				val_type val = unorderedmap_.at(key);
				val_size = sizeof(val);
				return val;
			}
		} return NULL;
	}

	void del(key_type key){
		// delete a key/value pair
		unorderedmap_.erase(key);
		return;
	}

	index_type space_used() const{
		index_type total_size = 0;
		for (auto const& pair: unorderedmap_){
			index_type val_size = sizeof(pair.second);
			total_size += val_size; 
		}
		return total_size;


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

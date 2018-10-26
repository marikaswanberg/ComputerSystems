#include "cache.hh"
#include <cstring>
#include <unordered_map>
#include <tuple>


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
<<<<<<< HEAD:Homework2/cache.cc
=======
	~Impl(){
		auto it = unorderedmap_.begin();
		while (it != unorderedmap_.end()) {
			delete[] static_cast<const char*>(std::get<0>(it->second));
			it = unorderedmap_.erase(it);
		}
	}
>>>>>>> 41bf975ac4a113fc76cba35b53405a682efa0949:Hw2/cache.cc

	void set(key_type key, val_type val, index_type size){
<<<<<<< HEAD:Homework2/cache.cc
		val_type copy = new val_type[size];
		if ((memused_ + size) > maxmem_) {
			//evict
=======
		// First check if key already has a value in the cache. 
		// Delete old tuple if necessary
		index_type tmp = 1;
		if (get(key, tmp) != nullptr || tmp != 0) {
			del(key);
		}
		// Set the new value
		val_type copy = new char[size];
		std::memcpy((void*)copy, val, size);
		std::tuple<val_type, index_type, index_type> entry = std::make_tuple((void*)copy, size, newest_);
		while ((memused_ + size) > maxmem_) {
			evictor();
>>>>>>> 41bf975ac4a113fc76cba35b53405a682efa0949:Hw2/cache.cc
		}
		std::memcpy((void*)copy, val, size);
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
		}
		val_size = 0; 	// set val_size to 0 if we don't find the value
		return NULL;
	}
<<<<<<< HEAD:Homework2/cache.cc

	void del(key_type key){
		// delete a key/value pair
		unorderedmap_.erase(key);
		//how to find amount by which to decrement memused?
=======
	
	// This deletes a (key, tuple) entry from the map
	void del(key_type key){
		index_type val_size = 0;
		auto val = static_cast<const char*>(get(key, val_size));
		unorderedmap_.erase(key);
		delete[] val;
		memused_ -= val_size;
>>>>>>> 41bf975ac4a113fc76cba35b53405a682efa0949:Hw2/cache.cc
		return;
	}

	index_type space_used() const{
		return memused_;
<<<<<<< HEAD:Homework2/cache.cc


	}
=======
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

>>>>>>> 41bf975ac4a113fc76cba35b53405a682efa0949:Hw2/cache.cc
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
	// we don't need anything here because pimpl is a unique_ptr
}

// Add a <key, value> pair to the cache.
// If key already exists, it will overwrite the old value.
// Both the key and the value are to be deep-copied (not just pointer copied).
// If maxmem capacity is exceeded, sufficient values will be removed
// from the cache to accomodate the new value.
void Cache::set(key_type key, val_type val, index_type size){
	return pImpl_->set(key,val,size);
}

// Retrieve a pointer to the value associated with key in the cache,
// or NULL if not found.
// Sets the actual size of the returned value (in bytes) in val_size
Cache::val_type Cache::get(key_type key, index_type& val_size) const {
	return pImpl_->get(key,val_size);
}

// Delete an object from the cache, if it's still there
void Cache::del(key_type key){
	pImpl_->del(key);
}

// Compute the total amount of memory used up by all cache values (not keys)
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

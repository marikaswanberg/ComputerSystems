#include "cache.hh"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <map>


struct Cache::Impl {

        index_type maxmem_;
        evictor_type evictor_;
        hash_func hasher_;
        index_type bytes_used_;
	
	/*Allows the input of your own hash funct using lambda functions passed into the creation of umap*/
	//hash_func new_hash = [](hash_func hasher) {if (hasher != NULL) { return hasher; } else { return std::function<uint32_t(key_type)>;}}; 
        //std::unordered_map<std::string, const void*, hash_func> storage;
	
        std::unordered_map<std::string, const void*> storage;
	/*Using umap to store vals - allows it to be passed a hash func, used buckets to avoid collisions,
	and allows you to set your own load factor. (Parts 3,4 , and 5)*/
	std::map<std::string, uint32_t> key_bytes;
	/*Using an ordered map helps with eviction by giving quick access to largest space consumed*/

	Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
	/*Initialize with bytes_used = 0 to quickly tell mem usage
	use properties of umap to set load factor to .5 (dynamic resizing)*/
        :maxmem_(maxmem), evictor_(evictor), hasher_(hasher), bytes_used_(0) 
        {	
		
		storage.max_load_factor(.5);
        }
        ~Impl() = default;

  // Add a <key, value> pair to the cache.
  // If key already exists, it will overwrite the old value.
  // If maxmem capacity is exceeded, sufficient values will be removed
  // from the cache to accomodate the new value.
        void set(key_type key, val_type val, index_type size)
        {
                while (bytes_used_ + size >= maxmem_)
                {
			evictor_FIFO();
                }
		storage[key] = val;
                key_bytes[key] = size;
                bytes_used_ += size;
		
		std::cout << "Inserting: Key: " << key << ", Value: " << val << '\n';

        }

  // Retrieve a pointer to the value associated with key in the cache,
  // or NULL if not found.
  // Sets the actual size of the returned value (in bytes) in val_size.
        val_type get(key_type key, index_type& val_size) const
        {
		/*Count func determines how many items are in the bucket at key - if 0, nothing has hashed there*/
		if (storage.count(key) > 0)
		{
                	val_type val_at = storage.at(key);
                	index_type actual_size = key_bytes.at(key);
                	val_size = actual_size;
			std::cout << "Retrieving: Key: " << key << ", Value: " << val_at << '\n';
			return val_at;
		}
		else
		{
			std::cout << "Key: " << key << " not found." << '\n';
			return 0;	
		}
        }

  // Delete an object from the cache, if it's still there
        void del(key_type key)
        {
		if (storage.count(key) == 0)
		{
			std::cout << "Key not stored in cache." << '\n';
		}
		else
		{
			index_type byte_size = key_bytes.at(key);
                	bytes_used_ -= byte_size;
                	storage.erase(key);
                	key_bytes.erase(key);
			std::cout << "Key: " << key << " successfully deleted." << '\n';
        	}

	}

  // Compute the total amount of memory used up by all cache values (not keys)
        index_type space_used() const
        {
                return bytes_used_;
        }
  
  //Evict the first key inserted to the cache
	void evictor_FIFO()
	{
		
		key_type first_key = key_bytes.begin()->first;
		/*-> first gives the key value of the first k/v pair in key_bytes, which is an ordered map so we evict the first value that was inserted*/
		del(first_key);
	
	}
};


// Create a new cache object with a given maximum memory capacity.
Cache::Cache(index_type maxmem,
        evictor_type evictor,
        hash_func hasher)

        :pImpl_(new Impl(maxmem, evictor, hasher))
{
}

Cache::~Cache()
{
}

/*All refs for pImpl below here, actual functionality written above*/

void Cache::set(key_type key, val_type val, index_type size)
{

        pImpl_ ->set(key, val, size);
}


Cache::val_type Cache::get(key_type key, index_type& val_size) const
{
        return pImpl_ ->get(key, val_size);
}


void Cache::del(key_type key)
{

        pImpl_ ->del(key);
}


Cache::index_type Cache::space_used() const
{
        return pImpl_ ->space_used();

}
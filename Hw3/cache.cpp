#include "cache.h"
#include <cstdlib>
#include <unordered_map>
#include <map>
#include <iostream>
#include <vector>

#include "evictor_fifo.h"
// #include "evictor_lru.h"

using evictor_obj_type = FIFO;

/*
 * CACHE :: IMPLEMENTATION
 */
class Cache::Impl {

    index_type   maxmem_;   //
    index_type   memused_;  // 
    evictor_type evictor_;  // () -> index_type
    hash_func    hasher_;   // key_type -> index_type
    
    std::unordered_map<std::string, std::pair<val_type, index_type>>
        cache_; // key => value, size
    
    bool USING_CUSTOM_EVICTION_ = false;
    evictor_obj_type * evictor_obj_;
    
    bool USING_RESIZING_ = false;
    index_type RESIZE_THRESHOLD_PERCENT_ = 75;
    
public:
        

    Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
    : maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0)
    {
        evictor_obj_ = new evictor_obj_type();
    }
    
    void
    del_largest() {
        // find the key of the smallest value
        key_type * key_max;
        index_type size_max = 0;
        
        for (auto& it : cache_) {
            key_type    key = it.first;
            index_type size = it.second.second;
            // first iteration
            if (size_max == 0) {
                key_max  = &key;
                size_max = size;
            }
            // subsequent iterations
            else if (size > size_max) {
                key_max  = &key;
                size_max = size;
            }
        }
        // delete the smallest value from the cache
        cache_.erase(*key_max);
        memused_ -= size_max;
    }
    
    void
    resize_cache() {
        maxmem_ *= 2;
    }
    
    bool
    contains(key_type key) const {
        return cache_.find(key) != cache_.end();
    }
    
    void
    check_eviction(index_type size) {
        // RESIZING
        // cache full passed resize threshold, so need to resize
        if (USING_RESIZING_) {
            // resize until fits
            while (100 * memused_ / maxmem_ >= RESIZE_THRESHOLD_PERCENT_) {
                resize_cache();
            }    
        }
        // EVICTION
        // if we added this new entry, the cache would be
        // full, so need to make room
        else {
            while (memused_ + size > maxmem_) {
                // custom eviction
                if (USING_CUSTOM_EVICTION_) {
                    key_type key_next = evictor_obj_->evict_next();
                    del(key_next); // remove from cache
                }
                // vanilla eviction
                else {
                    del_largest();
                }
            }
        }
        // adding new value will increase memused
        memused_ += size;
    }
    
    void
    set(key_type key, val_type val, index_type size) {
        // no amount of eviction will work...
        if (size > maxmem_ && !USING_RESIZING_) {
            throw "size too big for cache :/";
        }
        // key not already in cache, or the entry in the cache
        // that will be overwritten is at least as big as the
        // new entry
        if (!contains(key) || cache_.at(key).second >= size) {
            check_eviction(size);
        }
        cache_[key] = std::make_pair(val, size);
        if (USING_CUSTOM_EVICTION_) {
            evictor_obj_->push(key);
        }
    }
    
    val_type
    get(key_type key, index_type val_size) {
        // key is in cache
        if (contains(key)) {
            evictor_obj_->visit(key);
            return cache_.at(key).first;
        }
        // key not in cache
        throw "key not contained in cache >:(";
    }
    
    void
    del(key_type key) {
        // if key is in cache
        if (contains(key)) {
            auto it = cache_.at(key);
            index_type size = it.second;
            cache_.erase(key); // delete from cache
            memused_ -= size;  // decrease memused
            
            if (USING_CUSTOM_EVICTION_) {
                evictor_obj_->del(key);
            }
        } else {
            throw "key not contained in cache >:(";
        }
    }
    
    index_type
    space_used() const {
        return memused_;
    }
    
};

// Create a new cache object with a given max memory capacity
Cache::Cache(
    index_type maxmem,
    evictor_type evictor,
    hash_func hasher)
    : pImpl_(new Impl(maxmem, evictor, hasher))
{}

// Cleanup cache
Cache::~Cache() {}

/*
 * Defer function calls to Impl
 */

void Cache::
set(key_type key, val_type val, index_type size) {
    pImpl_->set(key, val, size);
}

Cache::val_type Cache::
get(key_type key, index_type val_size) {
    pImpl_->get(key, val_size);
}

void Cache::
del(key_type key) {
    pImpl_->del(key);
}

Cache::index_type Cache::
space_used() const {
    pImpl_->space_used();
}
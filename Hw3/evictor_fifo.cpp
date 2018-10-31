
#include <cstdlib>
#include <vector>
#include <memory>
#include <iostream>

#include "evictor_fifo.h"

FIFO::FIFO() {
    
}

void FIFO::push(key_type key_const) {
    std::string key = key_const;
    queue.insert(queue.begin(), key);
}

key_type FIFO::evict_next() {
    // get last element, remove it, then return it
    key_type key = queue.back();
    queue.pop_back();
    return key;
}

void FIFO::del(key_type key_const) {
    std::string key = key_const;
    std::vector<std::string> queue_new;
    // filter out everything with the given key
    for (std::string k : queue) {
        if (k != key) {
            queue_new.push_back(k);
        }
    }
    // update queue
    queue = queue_new;
}

void FIFO::visit(key_type key) {
    // FIFO doesn't make use of this, both other evictors might
}
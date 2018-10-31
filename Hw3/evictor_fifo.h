
#include <cstdlib>
#include <vector>
#include <memory>

using key_type = const std::string;  // Keys for K-V pairs
using val_type = const void*;        // Values for K-V pairs
using index_type = uint32_t;         // Internal indexing to K-V elements

/*
 * FIFO: first in first out eviction policy
 * 
 * is basically just a modified stack
 */
class FIFO {
    
private:
    std::vector<std::string> queue;
    
public:
    FIFO();
    
    // add an item to keep track of
    void push(key_type key);
    
    // evicts next item from being tracked, at returns it
    key_type evict_next();
    
    // remove an item being tracked
    void del(key_type key);
    
    // mark an entry as visited (not used for FIFO)
    void visit(key_type key);
    
};
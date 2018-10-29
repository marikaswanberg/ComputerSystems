/*
This file tests our Cache.
 */
#include "cache.hh"
#include <iostream>
#include <assert.h>

// Make sure an empty cache has zero space used.
void
test_space_empty(){
  Cache mycache(10);
  assert(mycache.space_used() == 0);
  return;
}

//Make sure capacity increases when we set a new value
void
test_space_dyn_int(){
  Cache mycache(10);
  int value = 6;
  mycache.set("hello", &value, sizeof(char));
  Cache::index_type size = mycache.space_used();
  assert(size == sizeof(char));
  return;
}

// Make sure space_used increases when we set a new array as a value
void
test_space_dyn_array(){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  Cache::index_type size = mycache.space_used();
  assert(size == 10);
  return;
}

// Make sure space_used decreases when we delete elements
void
test_space_del(){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  mycache.del("hello");
  Cache::index_type size = mycache.space_used();
  assert(size == 0);
}

// Make sure set increments memused_ properly
void
test_space_inc(){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  int value = 20;
  mycache.set("hello2", &value, sizeof(int));
  Cache::index_type size = mycache.space_used();
  assert(size == 10 + sizeof(int));
}

// Make sure the get function updates the size parameter we pass
// and returns the correct value for a value in the cache.
void
test_get_inCache(){
  Cache mycache(10);
  Cache::index_type size = 0;
  int value = 6;
  mycache.set("hello", &value, sizeof(value));
  mycache.get("hello", size);
  assert(size == sizeof(value));
}

// Make sure the get function doesn't return anything when
// the value isn't in the cache and doesn't update the size parameter
void
test_get_notInCache(){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  assert(size == 0);
  assert(val == nullptr);
}

// Make sure the get function doesn't return a value that
// has been deleted from the cache and doesn't update the
// size parameter.
void
test_get_deleted(){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  assert(size == 0);
  assert(val == NULL);
}

// Make sure that we can't access an element that has been evicted
void
test_get_evicted(){
  Cache mycache(10 + 2*sizeof(int));
  char buf[10];
  mycache.set("test", buf, 10);
  int value = 6;
  mycache.set("test2", &value, sizeof(int));
  int value2 = 3;
  mycache.set("test3", &value2, sizeof(int));
  int value3 = 4; 
  mycache.set("test4", &value3, sizeof(int));
  Cache::index_type size = 1;
  Cache::val_type val = mycache.get("test", size); // this is the element that will get evicted
  assert(val == NULL); // test that we can't get the value we just deleted
  assert(size == 0);
  
}

// Test that the get function returns the correct value
// when we modify a key in the cache
void
test_get_mod(){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  int modify = 99;
  mycache.set("test", &modify, sizeof(int));
  Cache::val_type val = mycache.get("test", size);
  assert(size == sizeof(int));
  assert(val);
  //again, how to test that we are getting the right value??
}


// Test that the cache evicts properly
void
test_surpass_capacity(){
  Cache mycache(10 + 2*sizeof(int));
  char buf[10];
  mycache.set("test", buf, 10);
  int value = 6;
  mycache.set("test2", &value, sizeof(int));
  int value2 = 3;
  mycache.set("test3", &value2, sizeof(int));
  Cache::index_type size1 = mycache.space_used();
  // overflow cache
  int value3 = 4; 
  mycache.set("test4", &value3, sizeof(int));
  Cache::index_type size2 = mycache.space_used();
  assert(size1 == (10 + 2*sizeof(int)));
  assert(size2 == (3*sizeof(int)));
}

// Test that we adhere to FIFO
void
test_FIFO(){
  Cache mycache(10 + 2*sizeof(int));
  char buf[10];
  mycache.set("test", buf, 10);
  int value = 6;
  mycache.set("test2", &value, sizeof(int));
  int value2 = 3;
  mycache.set("test3", &value2, sizeof(int));
  Cache::index_type space_used1 = mycache.space_used();
  // make sure get doesn't affect the order of eviction
  Cache::index_type size;
  mycache.get("test2", size);
  //overflow cache
  int value3 = 4; 
  mycache.set("test4", &value3, sizeof(int));
  Cache::index_type space_used2 = mycache.space_used();
  // try to access value that was deleted
  Cache::index_type size2 = 1;
  Cache::val_type val2 = mycache.get("test", size2);
  assert(val2 == NULL);
  assert(size2 == 0);
  // make sure the memory useage is correct
  assert(space_used1 == (10 + 2*sizeof(int)));
  assert(space_used2 == (3*sizeof(int)));
}



int main(){
  //test memory management
  test_space_empty();
  test_space_dyn_int();
  test_space_dyn_array();
  test_space_inc();
  test_space_del();

  // test set and get functionality
  test_get_inCache();
  test_get_notInCache();
  test_get_deleted();
  test_get_mod();
  test_get_evicted();

  // test evictor
  test_surpass_capacity();
  test_FIFO();
  std::cout << "All tests passed." << std::endl;

	return 0;


}

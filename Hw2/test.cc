/*
This file tests our Cache. Do I really need to explain?
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

// Make sure capacity increases when we set a new value
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
  Cache mycache(10);
  char buf[100];
  mycache.set("hello", buf, 100);
  Cache::index_type size = mycache.space_used();
  assert(size == 100);
  return;
}

// Make sure space_used decreases when we delete elements
void
test_space_del(){
  Cache mycache(10);
  char buf[100];
  mycache.set("hello", buf, 100);
  mycache.del("hello");
  Cache::index_type size = mycache.space_used();
  assert(size == 0);
}

// Make sure the get function updates the size parameter we pass
// and returns the correct value for a value in the cache.
void
test_get_inCache(){
  Cache mycache(10);
  Cache::index_type size;
  int value = 6;
  mycache.set("hello", &value, sizeof(value));
  //Cache::val_type val = mycache.get("hello", size); // what should get() do in this case?
  assert(size == sizeof(value));
  // how to test that it's the right value?
}

// Make sure the get function doesn't return anything when
// the value isn't in the cache and doesn't update the size parameter

void
test_get_notInCache(){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  assert(size == 0);
  assert(val == nullptr); //???? what should this be
}

// Make sure the get function doesn't return a value that
// has been deleted from the cache and doesn't update the
// size parameter.
void
test_get_deleted(){
  Cache mycache(10);
  Cache::index_type size = 0;
  char buf[100];
  mycache.set("test", buf, 100);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  assert(size == 0);
  assert(val == NULL);
}

// Test that the get function returns the correct value
// when we modify a key in the cache
void
test_get_mod(){
  Cache mycache(10);
  Cache::index_type size = 0;
  char buf[100];
  mycache.set("test", buf, 100);
  int modify = 99;
  mycache.set("test", &modify, sizeof(int));
  Cache::val_type val = mycache.get("test", size);
  assert(size == sizeof(int));
  assert(val);
  //again, how to test that we are getting the right value??
}

int main(){

	Cache mycache(10);
	int value = 6;
	char buf[100];
	mycache.set("hello",&value, sizeof(char));
	Cache::index_type first = mycache.space_used();
	mycache.set("hello2",buf, 100);
	Cache::index_type second = mycache.space_used();
	std::cout << "First size: " << first << std::endl;
	std::cout << "Second size: " << second << std::endl;

	Cache::index_type size1 = 0;
	Cache::index_type size2 = 0;
	Cache::val_type val = mycache.get("hello", size1);
	Cache::val_type val2 = mycache.get("hello2", size2);
	Cache::val_type val3 = mycache.get("hello3", size2);
	std::cout << val << " , " << val2 << " , " << val3 << std::endl;

	test_space_empty();
	test_space_dyn_int();
	test_space_dyn_array();
	test_get_inCache();
	test_get_notInCache();
	test_get_deleted();
	return 0;


}

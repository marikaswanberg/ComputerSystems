/*
This file tests our Cache.
 */
#include "cache.hh"
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch2.hpp"



TEST_CASE("Cache is empty", "[space_used]"){
  Cache mycache(10);
  REQUIRE(mycache.space_used() == 0);
}


TEST_CASE("space_used increases when we set a new value", "[space_used]"){
  Cache mycache(10);
  int value = 6;
  mycache.set("hello", &value, sizeof(char));
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == sizeof(char));
}


TEST_CASE("space_used decreases when we delete elements.", "[space_used]"){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  mycache.del("hello");
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == 0);
}

// Make sure set increments memused_ properly

TEST_CASE("Increments memused_ properly.", "[space_used]"){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  int value = 20;
  mycache.set("hello2", &value, sizeof(int));
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == 10 + sizeof(int));
}




TEST_CASE("Get updates the size parameter we pass to it.", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  int value = 6;
  mycache.set("hello", &value, sizeof(value));
  mycache.get("hello", size);
  REQUIRE(size == sizeof(value));
}

// // need to test that the correct value for a value in the cache.

// Make sure the get function doesn't return anything when
// the value isn't in the cache and doesn't update the size parameter

TEST_CASE("When value not in cache, get returns nullptr.", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  // assert(size == 0)
  REQUIRE(val == nullptr);
}

// Make sure the get function doesn't return a value that
// has been deleted from the cache and doesn't update the
// size parameter.

TEST_CASE("Get does not return deleted values.", "[get]"){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  // assert(size == 0);
  REQUIRE(val == NULL);
}

// Make sure that we can't access an element that has been evicted

TEST_CASE("Can't perform get on an element that has been evicted.", "[get]"){
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
  REQUIRE(val == NULL); // test that we can't get the value we just deleted
  // assert(size == 0);
  
  }

// Test that the get function returns the correct value
// when we modify a key in the cache

TEST_CASE("Get function returns the correct value after key is modified", "[get]"){
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

TEST_CASE("Cache evicts a single item properly", "[evictor]"){
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

// // Test that we adhere to FIFO
// void
// test_FIFO(){
//   Cache mycache(10 + 2*sizeof(int));
//   char buf[10];
//   mycache.set("test", buf, 10);
//   int value = 6;
//   mycache.set("test2", &value, sizeof(int));
//   int value2 = 3;
//   mycache.set("test3", &value2, sizeof(int));
//   Cache::index_type space_used1 = mycache.space_used();
//   // make sure get doesn't affect the order of eviction
//   Cache::index_type size;
//   mycache.get("test2", size);
//   //overflow cache
//   int value3 = 4; 
//   mycache.set("test4", &value3, sizeof(int));
//   Cache::index_type space_used2 = mycache.space_used();
//   // try to access value that was deleted
//   Cache::index_type size2 = 1;
//   Cache::val_type val2 = mycache.get("test", size2);
//   assert(val2 == NULL);
//   assert(size2 == 0);
//   // make sure the memory useage is correct
//   assert(space_used1 == (10 + 2*sizeof(int)));
//   assert(space_used2 == (3*sizeof(int)));
// }



// int main(){
//   //test memory management
//   test_space_empty();
//   test_space_dyn_int();
//   test_space_dyn_array();
//   test_space_inc();
//   test_space_del();

//   // test set and get functionality
//   test_get_inCache();
//   test_get_notInCache();
//   test_get_deleted();
//   test_get_mod();
//   test_get_evicted();

//   // test evictor
//   test_surpass_capacity();
//   test_FIFO();
//   std::cout << "All tests passed." << std::endl;

// 	return 0;


// }

/*
This file tests our Cache.
 */
#include "cache.hh"

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


TEST_CASE("When value not in cache, get returns nullptr.", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  REQUIRE(val == nullptr);
}


TEST_CASE("Get does not return deleted values.", "[get]"){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  REQUIRE(val == NULL);
}


TEST_CASE("Can't get an element that has been evicted.", "[get]"){
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
}


TEST_CASE("Get function returns the correct value after key is modified", "[get]"){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  int modify = 99;
  mycache.set("test", &modify, sizeof(int));
  int val = mycache.get("test", size);
  assert(size == sizeof(int));
  assert(val);
  //again, how to test that we are getting the right value??
}


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


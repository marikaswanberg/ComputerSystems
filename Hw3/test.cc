/*
This file tests our Cache.
 */
#include "cache.hh"

#define CATCH_CONFIG_MAIN
#include "catch2.hpp"


// cache is empty upon initialization
TEST_CASE("test_init_empty", "[space_used]"){
  Cache mycache(10);
  REQUIRE(mycache.space_used() == 0);
}

// memused increases properly when we set a new value
TEST_CASE("test_inc_memused", "[space_used]"){
  Cache mycache(10);
  int value = 6;
  mycache.set("hello", &value, sizeof(value));
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == sizeof(value));
}

// memused decreases properly when we delete elements
TEST_CASE("test_dec_memused", "[space_used]"){
  Cache mycache(20);
  char buf[10];
  mycache.set("hello", buf, 10);
  mycache.del("hello");
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == 0);
}

// Get updates the valsize parameter we pass to it.
TEST_CASE("test_get_valsize", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  int value = 6;
  mycache.set("hello", &value, sizeof(value));
  mycache.get("hello", size);
  REQUIRE(size == sizeof(value));
}

// // need to test that the correct value for a value in the cache.

// When value not in cache, get returns nullptr.
TEST_CASE("test_get_not_in_cache", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  REQUIRE(val == nullptr);
}

// Get does not return deleted values.
TEST_CASE("test_get_deleted", "[get]"){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  mycache.set("test", buf, 10);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  REQUIRE(val == NULL);
}

// Can't get an element that has been evicted.
TEST_CASE("test_get_evicted", "[get, evictor]"){
  Cache mycache(sizeof(int)); // only room for one int
  int value = 2018;
  mycache.set("evicted", &value, sizeof(value);
  int value2 = 2019;
  mycache.set("evictor", &value2, sizeof(value2)); // overflow the cache
  Cache::index_type size = 1;
  Cache::val_type val = mycache.get("evicted", size); // this is the element that got evicted
  REQUIRE(val == NULL); // test that we can't get the value we just evicted
}

// Get function returns the correct value after key is modified
TEST_CASE("test_get_modified", "[get]"){
  Cache mycache(20);
  Cache::index_type size = 0;
  char buf[10];
  int modify = 99;
  mycache.set("test", buf, 10);
  mycache.set("test", &modify, sizeof(int)); // change value for "test" key
  int val = mycache.get("test", size);
  REQUIRE(size == sizeof(int));
  REQUIRE(*val == 99);
}

// Cache evicts two items when necessary
TEST_CASE("test_set_evict", "[evictor, set]"){
  Cache mycache(2);
  char first[1] = "h";
  char second[1] = "i";
  char evictor[2] = "hi"; 
  mycache.set("first", first, sizeof(first));
  mycache.set("second", second, sizeof(second));
  mycache.set("evictor", evictor, sizeof(evictor)); // this will necessarily evict both ints regardless of the eviction policy
 
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == sizeof(evictor));
  // we use chars because they are defined to have size 1 across all operating systems.
}


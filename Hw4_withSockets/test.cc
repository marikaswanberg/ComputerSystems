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
  Cache mycache2(100);
  std::string value = "marika";
  mycache2.set("hello", &value, sizeof(value));
  Cache::index_type size = mycache2.space_used();
  REQUIRE(size == sizeof(value));
}

//memused increases properly after multiple sets
TEST_CASE("test_mult_inc_memused", "[space_used]"){
  Cache mycache(10);
  std::string first = "h";
  std::string second = "i";
  mycache.set("first", &first, sizeof(first));
  mycache.set("second", &second, sizeof(second));
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == sizeof(first)+sizeof(second));
  
}

// memused decreases properly when we delete elements
TEST_CASE("test_dec_memused", "[space_used]"){
  Cache mycache(100);
  std::string buf = "this will be deleted";
  mycache.set("hello", &buf, sizeof(buf));
  mycache.del("hello");
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == 0);
}

// Get updates the valsize parameter we pass to it.
TEST_CASE("test_get_valsize", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  std::string value = "eitan";
  mycache.set("hello", &value, sizeof(value));
  mycache.get("hello", size);
  REQUIRE(size == sizeof(value));
}

// need to test that the correct value for a value in the cache.
// Get is grabbing the correct value.
TEST_CASE("test_get_general", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  std::string value = "rocks";
  mycache.set("computer systems", &value, sizeof(value));
  const std::string* val = static_cast<const std::string*>(mycache.get("computer systems", size));
  REQUIRE(*val == "rocks");
}

// When value not in cache, get returns nullptr.
TEST_CASE("test_get_not_in_cache", "[get]"){
  Cache mycache(10);
  Cache::index_type size = 0;
  Cache::val_type val = mycache.get("hello", size);
  REQUIRE(val == nullptr);
}

// Get does not return deleted values.
TEST_CASE("test_get_deleted", "[get]"){
  Cache mycache(100);
  Cache::index_type size = 0;
  std::string to_delete = "this will be deleted!";
  mycache.set("test", &to_delete, 10);
  mycache.del("test");
  Cache::val_type val = mycache.get("test", size);
  REQUIRE(val == NULL);
}

//Can't get an element that has been evicted. (Run this test on a server with a memsize input of 32)
TEST_CASE("test_get_evicted", "[get, evictor]"){
  std::string value = "2018";
  std::cout << "Run test_get_evicted on a server with memsize input: " << sizeof(value) << std::endl;
  Cache mycache(32); // only room for one string
  mycache.set("evicted", &value, sizeof(value));
  std::string value2 = "2019";
  mycache.set("evictor", &value2, sizeof(value2)); // overflow the cache
  Cache::index_type size = 1;
  Cache::val_type val = mycache.get("evicted", size); // this is the element that got evicted
  REQUIRE(val == NULL); // test that we can't get the value we just evicted
}

// Get function returns the correct value after key is modified
TEST_CASE("test_get_modified", "[get]"){
  Cache mycache(100);
  Cache::index_type size = 0;
  std::string val = "hello";
  std::string modify = "hi";
  mycache.set("test", &val, sizeof(val));
  mycache.set("test", &modify, sizeof(modify)); // change value for "test" key
  const std::string* changed = static_cast<const std::string*>(mycache.get("test", size));
  REQUIRE(size == sizeof(modify));
  REQUIRE(*changed == modify);
}

// Cache evicts two items when necessary 
// (This won't pass for client/server project. Strings are always 32 bytes so there's no need to evict 2 strings to add 1.)
TEST_CASE("test_set_evict", "[evictor, set]"){
  std::string first = "h";
  std::string second = "i";
  std::string third = "hello there!";
  Cache mycache(64);
  mycache.set("first", &first, sizeof(first));
  mycache.set("second", &second, sizeof(first));
  mycache.set("evictor", &third, sizeof(third)); // this will necessarily evict both regardless of the eviction policy
  Cache::index_type size = mycache.space_used();
  REQUIRE(size == sizeof(third));
}

//Tests that we cannot delete an item that is not in the cache.
TEST_CASE("test_invalid_delete", "[del]"){
  Cache mycache(100);
  std::string test = "a";
  mycache.set("test", &test, sizeof(test));
  mycache.del("not_test");
  REQUIRE(mycache.space_used() == sizeof(test));
 }

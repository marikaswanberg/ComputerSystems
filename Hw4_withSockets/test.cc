/*
This file tests our Cache.
 */
#include "cache.hh"

#define CATCH_CONFIG_MAIN
#include "catch2.hpp"


// cache is empty upon initialization
// TEST_CASE("test_init_empty", "[space_used]"){
//   std::cout << "test_init_empty" << std::endl;
//   Cache mycache(10);
//   REQUIRE(mycache.space_used() == 0);
// }

// // memused increases properly when we set a new value
// TEST_CASE("test_inc_memused", "[space_used]"){
//   std::cout << "test_inc_memused" << std::endl;
//   Cache mycache2(100);
//   std::string value = "marika";
//   mycache2.set("hello", &value, sizeof(value));
//   std::cout << "passed set, starting spaceused" << std::endl;
//   Cache::index_type size = mycache2.space_used();
//   std::cout << "finished space_used" << std::endl;
//   REQUIRE(size == sizeof(value));
// }

// //memused increases properly after multiple sets
// TEST_CASE("test_mult_inc_memused", "[space_used]"){
//   std::cout << "test_mult_inc_memused" << std::endl;
//   Cache mycache(10);
//   std::string first = "h";
//   std::string second = "i";
//   mycache.set("first", &first, sizeof(first));
//   mycache.set("second", &second, sizeof(second));
//   Cache::index_type size = mycache.space_used();
//   REQUIRE(size == sizeof(first)+sizeof(second));
  
// }

// // memused decreases properly when we delete elements
// TEST_CASE("test_dec_memused", "[space_used]"){
//   std::cout << "test_dec_memused" << std::endl;
//   Cache mycache(100);
//   std::string buf = "this will be deleted";
//   mycache.set("hello", &buf, sizeof(buf));
//   mycache.del("hello");
//   Cache::index_type size = mycache.space_used();
//   REQUIRE(size == 0);
// }

// // Get updates the valsize parameter we pass to it.
// TEST_CASE("test_get_valsize", "[get]"){
//   Cache mycache(10);
//   Cache::index_type size = 0;
//   std::string value = "eitan";
//   mycache.set("hello", &value, sizeof(value));
//   mycache.get("hello", size);
//   REQUIRE(size == sizeof(value));
// }

// // need to test that the correct value for a value in the cache.
// // Get is grabbing the correct value.
// TEST_CASE("test_get_general", "[get]"){
//   Cache mycache(10);
//   Cache::index_type size = 0;
//   std::string value = "rocks";
//   mycache.set("computer systems", &value, sizeof(value));
//   const std::string* val = static_cast<const std::string*>(mycache.get("computer systems", size));
//   REQUIRE(*val == "rocks");
// }

// // When value not in cache, get returns nullptr.
// TEST_CASE("test_get_not_in_cache", "[get]"){
//   Cache mycache(10);
//   Cache::index_type size = 0;
//   Cache::val_type val = mycache.get("hello", size);
//   REQUIRE(val == nullptr);
// }

// // Get does not return deleted values.
// TEST_CASE("test_get_deleted", "[get]"){
//   std::cout << "test_get_deleted" << std::endl;
//   Cache mycache(100);
//   Cache::index_type size = 0;
//   std::string to_delete = "this will be deleted!";
//   mycache.set("test", &to_delete, 10);
//   mycache.del("test");
//   Cache::val_type val = mycache.get("test", size);
//   REQUIRE(val == NULL);
// }

// // Can't get an element that has been evicted. (Evictor not working properly for client)
// TEST_CASE("test_get_evicted", "[get, evictor]"){
//   std::string value = "2018";
//   Cache mycache(sizeof(value)); // only room for one int
//   mycache.set("evicted", &value, sizeof(value));
//   std::string value2 = "2019";
//   mycache.set("evictor", &value2, sizeof(value2)); // overflow the cache
//   Cache::index_type size = 1;
//   Cache::val_type val = mycache.get("evicted", size); // this is the element that got evicted
//   REQUIRE(val == NULL); // test that we can't get the value we just evicted
// }

// // Get function returns the correct value after key is modified
// TEST_CASE("test_get_modified", "[get]"){
//   Cache mycache(100);
//   Cache::index_type size = 0;
//   std::string val = "hello";
//   std::string modify = "hi";
//   mycache.set("test", &val, sizeof(val));
//   mycache.set("test", &modify, sizeof(modify)); // change value for "test" key
//   const std::string* changed = static_cast<const std::string*>(mycache.get("test", size));
//   REQUIRE(size == sizeof(modify));
//   REQUIRE(*changed == modify);
// }

// // Cache evicts two items when necessary (Evictor not working properly for client)
// TEST_CASE("test_set_evict", "[evictor, set]"){
//   Cache mycache(4);
//   char first[2] = "h";
//   char second[2] = "i";
//   char evictor[4] = "hi!"; 
//   mycache.set("first", first, sizeof(first));
//   mycache.set("second", second, sizeof(second));
//   mycache.set("evictor", evictor, sizeof(evictor)); // this will necessarily evict both ints regardless of the eviction policy
//   Cache::index_type size = mycache.space_used();
//   REQUIRE(size == sizeof(evictor));
//   // we use chars because they are defined to have size 1 across all operating systems.
// }

 TEST_CASE("test_invalid_delete", "[del]"){
  Cache mycache(100);
  std::string test = "a";
  mycache.set("test", test, sizeof(test));
  mycache.del("not_test");
  REQUIRE(mycache.space_used() == sizeof(test));
 }

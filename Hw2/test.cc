
#include "cache.hh"
#include <iostream>


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
	return 0;


}
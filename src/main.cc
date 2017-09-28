#include <lru.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

//! Represents user data in the below tests.  
class item  {
public:
  //!
  int value;

  //!
  string name;
  
  virtual void dump(std::string msg="") const {
    if ( msg.length() ) {
	std::cout << msg << std::endl;
    }
    std::cout << "this  = " << std::hex << this << std::endl; 
    std::cout << "Value = 0x" << std::hex << value << std::endl; 
  }
};

//! Calculates the hash for an item object 
int hash(const item& data) {
  return data.value;
}

//!*************************************
int main(int argc, const char* argv[]) {
  LRU<string, class item> lru;

  item  user_item;
  int testndx = 1;
  
  user_item.name = "itemA";
  user_item.id   = 0x1111;
  assert(lru.put(user_item.name, user_item));
  assert(lru.count()==1);
  std::cout << testndx++ << " Add 1 item and confirm 1 item in the LRU." << std::endl;

  user_item.name = "itemB";
  assert(lru.put(user_item.name, user_item));
  assert(lru.count()==2);
  std::cout << testndx++ << " Add 2nd item and confirm 2 items in the LRU." << std::endl;

  user_item.name = "itemC";
  assert(lru.put(user_item.name, user_item));
  assert(lru.count()==3);
  std::cout << testndx++ << " Add 3rd item and confirm 3 items in the LRU." << std::endl;

  user_item.name = "itemA";
  user_item.id   = 0x222;
  assert(lru.put(user_item.name, user_item));
  assert(lru.count()==4);
  std::cout << testndx++ << " Add item w/ duplicate key and confirm insert and 4 items in the LRU." << std::endl;

  assert(lru.get(0x111, user_item));
  assert(user_item.name==0x111);
  std::cout << testndx++ << " Confirm item.value=0x111 is in the LRU." << std::endl;
  
  lru.max_size(2);
  assert(2 == lru.count());
std::cout << testndx++ << " Set maximum size to 2 and confirm LRU size is reduced to 2." << std::endl;
 
  assert(!lru.get(0x133, user_item));
  std::cout << testndx++ << " Confirm item.value=0x133 is NOT in the LRU." << std::endl;
 
}
 

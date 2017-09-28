#include <lru.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

//! Represents user data in the below tests.  
class item  {
public:
  //!
  int id;

  //!
  string name;
  
  virtual void dump(std::string msg="") const {
    if ( msg.length() ) {
	std::cout << msg << std::endl;
    }
    std::cout << "this = " << std::hex << this << std::endl; 
    std::cout << "ID   = 0x" << std::hex << id << std::endl; 
    std::cout << "Name = " << name << std::endl; 
  }
};

//! Calculates the hash for an item object 
int hash(const item& data) {
  return data.id;
}

//!*************************************
int main(int argc, const char* argv[]) {

  item  user_item;
  int   testndx = 1;

  {
    LRU<string, class item> lru;
    std::cout << testndx++
	      << " - put() an item and confirm w/ get()."
	      << std::endl;
    user_item.name = "itemA";
    user_item.id   = 0x1111;
    assert(lru.put(user_item.name, user_item));
    assert(lru.count()==1);
    user_item.id   = 0x0;
    assert(lru.get(user_item.name, user_item));
    assert(user_item.id=0x1111);
  
    std::cout << testndx++ << " - put() a 2nd item and confirm w/ get()" << std::endl;
    user_item.name = "itemB";
    user_item.id   = 0x2222;
    assert(lru.put(user_item.name, user_item));
    assert(lru.count()==2);

    std::cout << testndx++ << " - put() 3rd item and confirm w/ get()." << std::endl;
    user_item.name = "itemC";
    assert (lru.put(user_item.name, user_item));
    assert(lru.count()==3);

    std::cout << testndx++ << " - get() 1st item and confirm." << std::endl;
    user_item.name = "itemA";
    assert(lru.get(user_item.name, user_item));
    assert(user_item.id==0x1111);

#if 0  
    std::cout << testndx++ << " - Set maximum size to 2 and confirm LRU size is reduced to 2." << std::endl;
    lru.max_size(2);
    assert(2 == lru.count());
#endif
  
    std::cout << testndx++ << " - get() item using invalid key, should fail." << std::endl;
    user_item.name = "itemD";
    assert(!lru.get(user_item.name, user_item));

    std::cout << testndx++ << " - get() itemB and confirm." << std::endl;
    user_item.name = "itemB";
    user_item.id   = 0;
    lru.get(user_item.name, user_item);
    assert(lru.get(user_item.name, user_item));
    assert(user_item.id==0x2222);
  }
}
 

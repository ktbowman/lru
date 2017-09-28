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
  int id2;

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

struct PairHash {
  size_t operator() (const std::pair<int, int> &k) const {
    std::hash<int> hasher;
    return hasher(k.first+k.second);
  }
};

struct PairEqual {
  bool operator() (const std::pair<int, int> &k, const std::pair<int, int> &l) const {
    return (k.first==l.first && k.second==l.second);
  }
};

//!*************************************
int main(int argc, const char* argv[]) {

  item  user_item;
  int   testndx = 1;

  // Test with T=class item
  {
    LRU<std::string,
	class item,
	std::hash<std::string>,
	std::equal_to<std::string>> lru;

    std::cout << testndx++
	      << " - put() an item and confirm w/ get()."
	      << std::endl;
    user_item.name = string("itemA");
    user_item.id   = 0x1111;
    assert(lru.put(user_item.name, user_item));
    assert(lru.count()==1);
    user_item.id   = 0x0;
    assert(lru.get(user_item.name, user_item));    
    assert(user_item.id=0x1111);
  
    std::cout << testndx++ << " - put() a 2nd item and confirm w/ get()" << std::endl;
    user_item.name = string("itemB");
    user_item.id   = 0x2222;
    assert(lru.put(user_item.name, user_item));
    assert(lru.count()==2);

    std::cout << testndx++ << " - put() 3rd item and confirm w/ get()." << std::endl;
    user_item.name = string("itemC");
    assert (lru.put(user_item.name, user_item));
    assert(lru.count()==3);
    
    std::cout << testndx++ << " - get() 1st item and confirm." << std::endl;
    user_item.name = string("itemA");
    assert(lru.get(user_item.name, user_item));
    assert(user_item.id==0x1111);

    std::cout << testndx++ << " - get() item using invalid key, should fail." << std::endl;
    user_item.name = "itemD";
    assert(!lru.get(user_item.name, user_item));

    std::cout << testndx++ << " - get() itemB and confirm." << std::endl;
    user_item.name = "itemB";
    user_item.id   = 0;
    lru.get(user_item.name, user_item);
    assert(lru.get(user_item.name, user_item));
    assert(user_item.id==0x2222);

    std::cout << testndx++ << " - clear() itemB and confirm." << std::endl;
    user_item.name = "itemB";
    lru.clear(user_item.name);
    assert(!lru.get(user_item.name, user_item));
    assert(user_item.id==0x0);

  }

  // Test with T=std::pair<int,int>
  {
    LRU<std::pair<int,int>,
	class item,
	PairHash,
	PairEqual> lru;
    
    std::cout << testndx++
	      << " - put() an item and confirm w/ get()."
	      << std::endl;
    user_item.id    = 0x1;
    user_item.id2   = 0x2;
    user_item.name  = "GOOD";
    assert(lru.put(make_pair(user_item.id, user_item.id2), user_item));
    assert(lru.count()==1);
    user_item.name  = "BAD";
    assert(lru.get(make_pair(user_item.id, user_item.id2), user_item));
    assert(user_item.id=0x1);
    assert(user_item.id=0x2);

    std::cout << testndx++
	      << " - put() an item with same key to create collision."
	      << std::endl;
    user_item.id    = 0x3;
    user_item.id2   = 0x0;
    user_item.name  = "GOOD";
    assert(lru.put(make_pair(user_item.id, user_item.id2), user_item));
    assert(lru.count()==2);
    
  }
}
 

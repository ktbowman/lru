#ifndef LRU_HPP_
#define LRU_HPP_

#include <unordered_map>
#include <list>
#include <cassert>
#include <iostream>
#include <iterator>

//! Used as item in std::list container. Maintains priority state. 
template <class K> class list_item {
public:

  //! Key to reference item in hash. Needed for O(1) removal on clear()
  K                  hash_key; 
};

//! Item saved in hash container. 
template <class K, class T> class hash_item {
public:

  //! User defined data type.
  T  value;
  
  //! Location of item the std::list. Relative location is based
  //! on time (LRU).
  class std::list<class list_item<K>>::iterator     list_it; 
  
};

#define MAX_LRU_SIZE 10

//! @brief LRU Cache
//  Supports:
//  - Supports duplicate keys with collisions.
//  - Limiting size.
//  - Adding item(s) with eviction as necessary
//  - Removing item(s) based on LRU logic.
template <class K, class T, class H, class E> class LRU {
protected:

  //! Maximum size of the LRU 
  int _max_size;

  //! Used to to store the LRU data, O(1) access 
  std::unordered_map<K, class hash_item<K,T>, H, E>  hash_cont;

  //! Used to manage LRU eviction logic
  std::list<class list_item<K>>                       list_cont;
  
public:

  //! Sets max Nnumber of items allowed.
  void max_size(unsigned int new_max_size);

  //! Number of items in the list and hash
  int count(void);
  
  //! Default constructor
  LRU(int _max_size=MAX_LRU_SIZE);

  //! Clears a T item entry
  void clear(const K& key);

  //! Retrieves a T item entry
  bool get(const K& key, T& item);

  //! Adds a T item entry
  // @Return true=success, false=failure 
  bool put(const K& key, const T& item);
  
};


template<class K, class T, class H, class E>
LRU<K,T,H,E>::LRU(int new_max_size):
  _max_size(new_max_size) {
}

template<class K, class T, class H, class E>
bool LRU<K,T,H,E>::get(const K& key, T& value){
  bool rc = true;
  class std::unordered_map<K, class hash_item<K,T>, H, E>::iterator hash_it;
  hash_it = hash_cont.find(key);

  if ( hash_cont.end() == hash_it ) {
    rc = false;
    std::cerr << __FUNCTION__ << "() - Failed to find item." << std::endl;
  } else {
    value = (hash_it->second).value;
    std::iter_swap((hash_it->second).list_it, list_cont.begin());
  }
  return rc;
}

template<class K, class T, class H, class E> 
bool LRU<K,T,H,E>::put(const K& key, const T& user_item){
  bool rc = true;

  assert(list_cont.size()==hash_cont.size());

  // Make free space in the list and map
  while ( list_cont.size() >= _max_size ) {
    hash_cont.erase(list_cont.back().hash_key);
    list_cont.pop_back();
  }
  assert(list_cont.size() == hash_cont.size());

  
  // Create the element for the linked list, maintains priority
  list_item<K> list_item_tmp;
  list_item_tmp.hash_key = key;
  list_cont.push_front(list_item_tmp);
  
  // Create the hash entry for the item 
  hash_item<K,T> hash_item_tmp;
  hash_item_tmp.list_it = list_cont.begin(); 
  hash_item_tmp.value   = user_item;
  hash_cont.insert(std::make_pair(key, hash_item_tmp));

  //std::cout << "list_cont.size() = " << list_cont.size() << std::endl;
  //std::cout << "hash_cont.size() = " << hash_cont.size() << std::endl;
  assert(list_cont.size()==hash_cont.size());
  
  return rc;

}

template<class K, class T, class H, class E>
void LRU<K,T,H,E>::clear(const K& key){
  assert(list_cont.size() == hash_cont.size());
  list_cont.erase(hash_cont[key].list_it);
  hash_cont.erase(key);
  assert(list_cont.size() == hash_cont.size());
}

template<class K, class T, class H, class E>
int LRU<K,T,H,E>::count() {
  assert(list_cont.size()==hash_cont.size());
    return(list_cont.size());
};

template<class K, class T, class H, class E>
void LRU<K,T,H,E>::max_size(unsigned int new_max_size) {
  
  _max_size = new_max_size;
  assert(list_cont.size()==hash_cont.size());
  while ( list_cont.size() > _max_size ) {
    hash_cont.erase(list_cont.back().hash_key);
    list_cont.pop_back();
  }
  assert(list_cont.size()==hash_cont.size());
};

#endif // LRU_HPP_

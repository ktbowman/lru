#ifndef LRU_HPP_
#define LRU_HPP_

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include <cassert>

// TODO - Add support for multiple items with same hash index
template <class K, class T> class list_item {
public:

  //! Needed for O(1) removal on clear()
  K                  hash_key; 
};

// TODO - Add support for multiple items with same hash index
//        Would 
template <class K, class T> class hash_item {
public:
  //!
  T  value;
  
  //! Needed for O(1) removal on clear()
  class std::list<class list_item<K,T>>::iterator     list_it; 
  
};

#define MAX_LRU_SIZE 10

//!
// Requirements:
// - Remove LRU item. Includes removing from the list and the hash
//   - Can be result of:
//     - clear()
//     - LRU eviction
// - Remove hash item. Includes removing from the list and the hash
//   - Can be result of:
//     - clear()
//     - LRU eviction
// - Add item. Includes adding item to the list and has 
template <class K, class T> class LRU {
protected:

  //! Maximum size of the LRU 
  int _max_size;

  //! Used to to store the LRU data, O(1) access 
  std::unordered_map<K, class hash_item<K,T>>  hash_cont;

  //! Used to manage LRU eviction logic
  std::list<class list_item<K,T>>               list_cont;
  
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
  bool put(const T& item);
  
};


template<class K, class T>
LRU<K,T>::LRU(int new_max_size):
  _max_size(new_max_size) {
}

template<class K, class T>
bool LRU<K,T>::get(const K& key, T& value){
  bool rc = true;
  class std::unordered_map<K, class hash_item<K,T>>::iterator it;
  it = hash_cont.find(key);

  // Disabling duplicates because of short amount oi time. To allow
  // duplicates would change hash_item to contain list of entries
  // instead of a single entry.
  if ( hash_cont.end() == it ) {
    rc = false;
  } else {
    hash_item<K,T> hash_item = it->second;
    list_item<K,T> list_item_tmp = *(hash_item.list_it);
    value = hash_item.value;
    list_cont.erase(hash_item.list_it);
    list_cont.push_front(list_item_tmp);
  }
  return rc;
}

template<class K, class T> 
bool LRU<K,T>::put(const T& user_item){
  bool rc = true;
  
  // Make free space in the list and map
  while ( list_cont.size() >= _max_size ) {
    K hash_key = list_cont.back().hash_key;
    hash_cont.erase(hash_key);
    list_cont.pop_back();
  }
  assert(list_cont.size() == hash_cont.size());

  // Currently not supporting duplicates, I know, lame.
  // Checking here so list and map are consistent. This is
  // Advertised as constant O(c) in most cases.
  K hash_key = hash(user_item);
  if ( hash_cont.end() != hash_cont.find(hash_key) ) {
    rc = false;
  }

  if ( rc ) {    
    // Create the element for the linked list, maintains priority
    list_item<K,T> list_item_tmp;
    list_item_tmp.hash_key = hash(user_item);
    list_cont.push_front(list_item_tmp);
    
    // Create the hash entry for the item 
    hash_item<K,T> hash_item_tmp;
    hash_item_tmp.list_it = list_cont.begin(); 
    hash_item_tmp.value   = user_item;
    hash_cont.insert(std::make_pair(hash(user_item), hash_item_tmp));
  }
  
  return rc;

}

template<class K, class T>
void LRU<K,T>::clear(const K& key){
  assert(list_cont.size() == hash_cont.size());
  K hash_value = hash(key);
  list_cont.erase(hash_cont[hash_value].list_it);
  hash_cont.erase(hash_value);
  assert(list_cont.size() == hash_cont.size());
}

template<class K, class T>
int LRU<K,T>::count() {
  assert(list_cont.size()==hash_cont.size());
    return(list_cont.size());
};

template<class K, class T>
void LRU<K,T>::max_size(unsigned int new_max_size) {
  
  _max_size = new_max_size;
  while ( list_cont.size() > _max_size ) {
    K hash_key = list_cont.back().hash_key;
    hash_cont.erase(hash_key);
    list_cont.pop_back();
  }
  assert(list_cont.size()==hash_cont.size());
};

#endif // LRU_HPP_

#ifndef LRU_HPP_
#define LRU_HPP_

#include <unordered_map>
#include <list>
#include <cassert>
#include <iostream>
#include <iterator>

// TODO - Add support for multiple items with same hash index
template <class K> class list_item {
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
  class std::list<class list_item<K>>::iterator     list_it; 
  
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

  // Disabling duplicates because of short amount oi time. To allow
  // duplicates would change hash_item to contain list of entries
  // instead of a single entry.
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

  // Currently not supporting duplicates, I know, lame.
  // Checking here so list and map are consistent. This is
  // Advertised as constant O(c) in most cases.
#if 0
  class std::unordered_map<K, class hash_item<K,T>>::iterator it;
  it = hash_cont.find(key);
  if ( (hash_cont.end() != it) && ()  ) {
    rc = false;
  }
#endif
  
  if ( rc ) {    
    // Create the element for the linked list, maintains priority
    list_item<K> list_item_tmp;
    list_item_tmp.hash_key = key;
    list_cont.push_front(list_item_tmp);
    
    // Create the hash entry for the item 
    hash_item<K,T> hash_item_tmp;
    hash_item_tmp.list_it = list_cont.begin(); 
    hash_item_tmp.value   = user_item;
    hash_cont.insert(std::make_pair(key, hash_item_tmp));
  }
  //std::cout << "list_cont.size() = " << list_cont.size() << std::endl;
  //std::cout << "hash_cont.size() = " << hash_cont.size() << std::endl;
  assert(list_cont.size()==hash_cont.size());
  
  return rc;

}

template<class K, class T, class H, class E>
void LRU<K,T,H,E>::clear(const K& key){
  assert(list_cont.size() == hash_cont.size());
  K hash_value = hash(key);
  list_cont.erase(hash_cont[hash_value].list_it);
  hash_cont.erase(hash_value);
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

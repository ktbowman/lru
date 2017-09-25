#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//! Class to find the first unique item in the list
//  TODO: Needs to be made template class for container type and element type.
class FindFirstUnique {
public:

  //! Enable/Disable debugging
  int debug;
  
  //! List of numbers provided by the user
  std::vector<int> original_vector;

  //! Default constructor
  FindFirstUnique() :
    debug(0) {
  }
  
  //! Loads data from stdin
  virtual bool load(void) {
    int number;
    while (std::cin >> number) {
      try {
	original_vector.push_back(number);
      } catch(const std::bad_alloc& e) {
        std::cout << "Allocation failed: " << e.what() << '\n';
      }
    }
    if ( debug ) {
      vector_print("Original Vector", original_vector);
    }
    return true;
  }

protected:
  //! Displays vector. For debugging. 
  virtual const void vector_print(const std::string descr,
                                    const std::vector<int>& v) {
    std::cout << std::endl << descr << std::endl;
    std::cout << "Size = " << v.size() << std::endl;
    for (std::vector<int>::const_iterator it=v.begin();
         it < v.end();
         it++) {
      std::cout << *it << std::endl;
    }
  }

  //! Sorts the vector 
  virtual void sort(vector<int>& original_vector,
                     vector<int>& sorted_vector) {
    
    // Sort it so are items are contiguous
    // O(N*LogN) QSort/Merge Sort
    sorted_vector = original_vector;
    std::stable_sort(sorted_vector.begin(), sorted_vector.end());

    if ( debug ) {
      vector_print("Sorted Vector", sorted_vector);
    }
  }

  //! 
  virtual void remove_duplicates(std::vector<int>& sorted_vector,
                                   std::vector<int>& unique_vector) {

    // Preallocate to prevent continuous vector resizing
    unique_vector.resize(sorted_vector.size());

    // Remove any duplicates ex. (1, 1, 2) -> (2) 
    // O(N)
    auto had_match   = false; // Represents match for trailing characters.
    int pb_count     = 0;     // Index for inplace insert with [] operator.to avoid allocs
    for (auto sorted_vector_it = sorted_vector.begin();
         sorted_vector_it<sorted_vector.end()-1;
         sorted_vector_it++) {

      if ( debug ) {
        std::cout << "Looking at " << *sorted_vector_it
                  << "<-->" << *(sorted_vector_it+1) << std::endl;
      }
      if ( *sorted_vector_it != *(sorted_vector_it+1) ) {
        if (!had_match ) {

	  try {
	    unique_vector.at(pb_count++) = *(sorted_vector_it);
	  } 
	  catch (const std::out_of_range& oor) {
	    std::cerr << "Out of Range error: " << oor.what() << '\n';
	  }	  
	  
          if ( debug ) {
            std::cout << "Setting " << *(sorted_vector_it+1) << std::endl;
          }
        }
        had_match = false;
      } else {
        had_match = true;
      }
    }

    // Handle forward match comparison at end of vector 
    if ( *(sorted_vector.end()-1) != *(sorted_vector.end()-2) ) {
      if ( debug ) {
        std::cout << "Setting " << *(sorted_vector.end()-1 ) << " "
                  << *(sorted_vector.end()-2 ) << std::endl;
	try {
	  unique_vector.at(pb_count++) = *(sorted_vector.end()-1);
	} 
	catch (const std::out_of_range& oor) {
	  std::cerr << "Out of Range error: " << oor.what() << '\n';
	}	  	
      }
    }

    // Free unused
    unique_vector.resize(pb_count);
    if ( debug ) {
      vector_print("Unique Vector", unique_vector);
    }
  }

public:
  
  //!
  virtual bool
  run(std::vector<int>::iterator& current_min) {
    
    std::vector<int> unique_vector(0);
    std::vector<int> sorted_vector;

    if ( 0 >= original_vector.size() ) {
      return false;
    }

    sort(original_vector, sorted_vector);

    remove_duplicates(sorted_vector, unique_vector);
    
    // Take each item from from the unique_vector, to find the earliest 
    // location in original_vector.
    // O(m*n)
    current_min=original_vector.end();
    for ( auto x : unique_vector ) {
      std::vector<int>::iterator it = std::find(original_vector.begin(),
                                                 original_vector.end(),
						 x);
      if ( original_vector.end() != it ) { 
        current_min = std::min(current_min, it);
      }
      if (  debug ) {
        std::cout << "x : " << x << std::endl; 
        std::cout << "Current min: " << *current_min << std::endl;
      }
    }
    return (original_vector.end() != current_min);
  }
};

//!*************************************
int main(int argc, const char* argv[]) {
  FindFirstUnique            find_first_unique;
  std::vector<int>::iterator item_it;
  std::string                 msg = "Undefined | Does not exist";

  find_first_unique.debug = 1;
  find_first_unique.load();
  
  if ( find_first_unique.run(item_it) ) {
    msg = std::to_string(*item_it);
  }
  
  std::cout << "First unique item in the list is: "
            << msg
            << std::endl;
}

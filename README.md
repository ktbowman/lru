## Build instructions
autoreconf --install && ./configure && make

## Example usage:
src/lru

## Test Plan
Tested against valgrind for leaks.

Using abstract data type T with Key (K) of string.

Tests include:
1 - put() an item and confirm w/ get().
2 - put() a 2nd item and confirm w/ get()
3 - put() 3rd item and confirm w/ get().
4 - get() 1st item and confirm.
5 - get() item using invalid key, should fail.
get() - Failed to find item.
6 - get() itemB and confirm.
7 - clear() itemB and confirm.
get() - Failed to find item.

Using abstract data type T with Key (K) of pair<int,int>.

8 - put() an item and confirm w/ get().
9 - put() an item with same key to create collision.

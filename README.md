# C Hash Table
Ensures that there is only one instance of each unique value.

This table is based on an [amazing tutorial](http://www.craftinginterpreters.com/hash-tables.html) by Bob Nystrom.

It uses a hashing function called [FNV-1a](http://www.isthe.com/chongo/tech/comp/fnv/).

It doesn't let you associate a value with a key, its only function is to make sure there is only one copy of a certain value.

If you want to associate a key with a value, you can easily add support for that on your own. I will make a more advanced hash table at some point, but you can also just fork this one and make you own changes, especially if you follow the tutorial I linked, which has all the features of a normal hash table, including deletion.

The usage is simple:

```c
#include "table.h"

// ...

table* t = table_create();

// ...

// table_get_entry will search for a matching value and make a new entry if there isn't one
// this guarantees that you won't have any duplicate values
const char* value = table_get_entry(t, data_str, length);

// ...

table_free(t);
```

The main function for this hash table is `table_get_entry`:
```c
const void* table_get_entry(table* t, const void* data, size_t size);
```
`t` is just a pointer to a table structure, `value` is a pointer to some kind of data, and `size` is the length (in bytes) of the data. It returns a pointer to some data, which will either be an existing match that was already in the table or a new entry if there was no match. This is how you can be sure there are no duplicate values.

`main.c` runs a benchmark for both the O(1) hashmap and a regular O(n) array search with 17,576 string entries each. The output usually looks something like this:

```
hash table time: 0.028826 seconds

array time: 1.669187 seconds

the map was 57.905606 times faster than the array

Program ended with exit code: 0
```

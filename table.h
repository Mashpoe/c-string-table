//
//  table.h
//  hashmap
//
//  Created by Mashpoe on 9/14/19.
//
//	Tables ensure that there is only one instance of each unique value
//

#ifndef table_h
#define table_h

#define TABLE_DEFAULT_CAPACITY		20
#define TABLE_MAX_LOAD				0.75f
#define TABLE_RESIZE_FACTOR			2

#include <stdlib.h>

typedef struct table table;

table* table_create(void);
void table_free(table* t);

// returns the value of either an existing entry or a new one if no matching value exists
const void* table_get_entry(table* t, const void* value, size_t size);

#endif /* table_h */

//
//  table.c
//  hashmap
//
//  Created by Mashpoe on 9/14/19.
//

#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


// values will be zero/false because of calloc
//union bucket_info {
//	// if a bucket's key is null, true means that it is a placeholder bucket upon which other buckets depend
//	bool tombstone;
//
//	// how many bytes is the value (obviously)
//	size_t size;
//};

typedef struct bucket {
	const void* value; // the key is also the value in this structure (we are just trying to deduplicate values)
	size_t size; // size of value (obviously)
	uint32_t hash;
} bucket;

typedef struct table {
	bucket* buckets; // bucket array
	int capacity;
	int count;
} table;

table* table_create(void) {
	table* t = malloc(sizeof(table));
	t->capacity = TABLE_DEFAULT_CAPACITY;
	t->buckets = calloc(TABLE_DEFAULT_CAPACITY, sizeof(bucket));
	t->count = 0;
	return t;
}

void table_free(table* t) {
	for (int i = 0; i < t->capacity; i++) {
		if (t->buckets[i].value != NULL) {
			free((void*)t->buckets[i].value);
		}
	}
	free(t);
}

uint32_t hash_data(const void* data, size_t size) {
	uint32_t hash = 2166136261u;
	for (size_t i = 0; i < size; i++) {
		hash ^= ((const char*)data)[i];
		hash *= 16777619;
		i++;
	}
	return hash;
}

// puts an old bucket into a resized table
void resize_entry(table* t, bucket* old_entry) {
	uint32_t index = old_entry->hash % t->capacity;
	for (;;) {
		bucket* entry = &t->buckets[index];
		
		if (entry->value == NULL) {
			*entry = *old_entry; // copy data from old entry
			break;
		}
		
		index = (index + 1) % t->capacity;
	}
}

void table_resize(table* t) {
	
	size_t old_capacity = t->capacity;
	bucket* old_buckets = t->buckets;
	
	t->capacity *= TABLE_RESIZE_FACTOR;
	t->buckets = calloc(t->capacity, sizeof(bucket)); // initializes everything to 0
	//printf("resize %i\n", t->capacity);
	
	for (int i = 0; i < old_capacity; i++) {
		bucket* old_bucket = &old_buckets[i];
		if (old_bucket->value == NULL) continue;
		
		resize_entry(t, old_bucket);
	}
	
	free(old_buckets);
}

bucket* find_entry(table* t, const void* value, size_t size, uint32_t hash) {
	uint32_t index = hash % t->capacity;
	for (;;) {
		bucket* entry = &t->buckets[index];
		
		if (entry->value == NULL) {
			
			//if (!b->info.tombstone) commented out because we don't use tombstones
			return entry;
			
		// check if the data is the same
		} else if (entry->size == size && memcmp(entry->value, value, size) == 0) {
			return entry;
		}
		
		//printf("collision\n");
		index = (index + 1) % t->capacity;
	}
}

const void* table_get_entry(table* t, const void* value, size_t size) {
	
	// hash the data
	uint32_t hash = hash_data(value, size);
	
	// find the appropriate entry
	bucket* entry = find_entry(t, value, size, hash);
	
	if (entry->value != NULL) {
		//printf("entry found\n");
		return entry->value;
	}
	
	// there is no entry yet, make sure there is space and make one
	if (t->count + 1 > TABLE_MAX_LOAD * t->capacity) {
		table_resize(t);
		// reassign entry, as the table was resized and it probably moved
		entry = find_entry(t, value, size, hash);
	}
	
	// populate the new entry
	void* entry_value = malloc(size);
	memcpy(entry_value, value, size); // make a copy of the data for the new entry
	entry->value = entry_value;
	entry->size = size;
	entry->hash = hash;
	t->count++;
	
	//printf("entry created\n");
	
	return entry->value;
}

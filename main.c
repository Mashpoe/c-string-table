//
//  main.c
//  hashmap
//
//  Created by Mashpoe on 9/13/19.
//

#include <stdio.h>
#include <string.h>
#include "table.h"

#include <time.h>


// forward declarations
typedef struct arr arr;
arr* arr_create(void);
const void* arr_get_entry(arr* a, const void* value, size_t size);
void arr_free(arr* a);


int main(int argc, const char * argv[]) {
	/*
	*   benchmark for hashmap vs searching through an array
	*/
	char str[3];
	clock_t begin, end;
	double time_spent_map, time_spent_arr;
	
	// hashmap benchmark
	begin = clock();

	table* hash_table = table_create();
	for (char a = 'a'; a <= 'z'; a++) {
		for (char b = 'a'; b <= 'z'; b++) {
			for (char c = 'a'; c <= 'z'; c++) {
				str[0] = a;
				str[1] = b;
				str[2] = c;

				table_get_entry(hash_table, str, 3);
				table_get_entry(hash_table, str, 3);
			}
		}
	}

	end = clock();
	time_spent_map = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("hash table time: %f seconds\n\n", time_spent_map);
	
	// array benchmark
	begin = clock();
	
	arr* array = arr_create();

	for (char a = 'a'; a <= 'z'; a++) {
		for (char b = 'a'; b <= 'z'; b++) {
			for (char c = 'a'; c <= 'z'; c++) {
				str[0] = a;
				str[1] = b;
				str[2] = c;

				arr_get_entry(array, (const char*)str, 3);
				arr_get_entry(array, (const char*)str, 3);
			}
		}
	}

	end = clock();
	time_spent_arr = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("array time: %f seconds\n\n", time_spent_arr);
	
	printf("the map was %f times faster than the array\n\n", time_spent_arr/time_spent_map);
	
	table_free(hash_table);
	arr_free(array);
	
	return 0;
}

typedef struct arr_bucket {
	const void* value;
	size_t size;
} arr_bucket;

struct arr {
	arr_bucket* buckets;
	int capacity;
	int count;
};

arr* arr_create(void) {
	arr* a = malloc(sizeof(arr));
	a->capacity = TABLE_DEFAULT_CAPACITY;
	a->buckets = calloc(TABLE_DEFAULT_CAPACITY, sizeof(arr_bucket));
	a->count = 0;
	return a;
}

const void* arr_get_entry(arr* a, const void* value, size_t size) {
	for (int i = 0; i < a->count; i++) {
		arr_bucket* entry = &a->buckets[i];
		
		if (entry->size == size && memcmp(entry->value, value, size) == 0) {
			//printf("entry found\n");
			return entry->value;
		}
	}
	
	if (a->count + 1 > TABLE_MAX_LOAD * a->capacity) {
		a->capacity *= TABLE_RESIZE_FACTOR;
		a->buckets = realloc(a->buckets, a->capacity * sizeof(arr_bucket));
		//printf("resize: %i\n", a->capacity);
	}
	// copy the new entry
	arr_bucket* entry = &a->buckets[a->count++];
	void* entry_value = malloc(size);
	memcpy(entry_value, value, size);
	entry->value = entry_value;
	entry->size = size;
	
	//printf("entry created\n");
	
	return entry;
}

void arr_free(arr* a) {
	for (int i = 0; i < a->count; i++) {
		free((void*)a->buckets[i].value);
	}
}

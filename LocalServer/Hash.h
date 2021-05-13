#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
}entry_t;

typedef struct hashtable_s {
	int size;
	struct entry_s **table;	
}hashtable_t;

hashtable_t *ht_create( int size );
int ht_hash( hashtable_t *hashtable, char *key );
entry_t *ht_newpair( char *key, char *value );
void ht_set( hashtable_t *hashtable, char *key, char *value );
char *ht_get( hashtable_t *hashtable, char *key );

#endif

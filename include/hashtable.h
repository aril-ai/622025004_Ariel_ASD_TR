#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "item.h"

#define HASH_SIZE 11

typedef struct HashNode {
    Item item;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *buckets[HASH_SIZE];
    int collisions;
} HashTable;

void initHashTable(HashTable *table);
int insertHash(HashTable *table, const Item *item);
HashNode *searchHash(const HashTable *table, int id, int *bucket, int *steps);
void freeHashTable(HashTable *table);
void runHashSearch(const ItemList *list);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "utils.h"

static int hashFunction(int id) {
    return (int)(((unsigned int)id * 31u + 7u) % HASH_SIZE);
}

void initHashTable(HashTable *table) {
    int i;

    for (i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->collisions = 0;
}

int insertHash(HashTable *table, const Item *item) {
    int bucket = hashFunction(item->id);
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));

    if (node == NULL) {
        return 0;
    }
    node->item = *item;
    if (table->buckets[bucket] != NULL) {
        table->collisions++;
    }
    node->next = table->buckets[bucket];
    table->buckets[bucket] = node;
    return 1;
}

HashNode *searchHash(const HashTable *table, int id, int *bucket, int *steps) {
    HashNode *current;

    *bucket = hashFunction(id);
    *steps = 0;
    current = table->buckets[*bucket];
    while (current != NULL) {
        (*steps)++;
        if (current->item.id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void freeHashTable(HashTable *table) {
    int i;
    HashNode *current;
    HashNode *next;

    for (i = 0; i < HASH_SIZE; i++) {
        current = table->buckets[i];
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        table->buckets[i] = NULL;
    }
}

void runHashSearch(const ItemList *list) {
    HashTable table;
    HashNode *found;
    int i, id, bucket, steps;

    if (list->count == 0) {
        printf("Belum ada item.\n");
        return;
    }
    initHashTable(&table);
    for (i = 0; i < list->count; i++) {
        if (!insertHash(&table, &list->data[i])) {
            printf("Memori tidak cukup untuk membangun hash table.\n");
            freeHashTable(&table);
            return;
        }
    }
    printf("Hash table dibangun: %d item, %d bucket, %d collision (chaining).\n",
           list->count, HASH_SIZE, table.collisions);

    id = readInt("ID yang dicari: ", 1, 1000000);
    found = searchHash(&table, id, &bucket, &steps);
    if (found == NULL) {
        printf("Item dengan ID %d tidak ditemukan (bucket %d, %d node ditelusuri).\n",
               id, bucket, steps);
    } else {
        printf("Ditemukan di bucket %d setelah menelusuri %d node.\n", bucket, steps);
        printf("ID: %d | Nama: %s | Kategori: %s | Stok: %d | Harga: %d\n",
               found->item.id, found->item.name, found->item.category,
               found->item.stock, found->item.price);
    }
    freeHashTable(&table);
}

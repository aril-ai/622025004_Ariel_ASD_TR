#ifndef SEARCHING_H
#define SEARCHING_H

#include "item.h"

int sequentialSearchByName(const ItemList *list, const char *keyword);
int binarySearchById(const ItemList *list, int id);
void runSequentialSearch(const ItemList *list);
void runBinarySearch(ItemList *list);

#endif

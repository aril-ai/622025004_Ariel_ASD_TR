#ifndef SORTING_H
#define SORTING_H

#include "item.h"

#define SORT_BY_ID 1
#define SORT_BY_NAME 2
#define SORT_BY_STOCK 3
#define SORT_BY_PRICE 4

void bubbleSortItems(Item *data, int count, int field, int ascending);
int mergeSortItems(Item *data, int count, int field, int ascending);
void runSortMenu(ItemList *list);

#endif

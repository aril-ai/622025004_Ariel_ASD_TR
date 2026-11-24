#ifndef ITEM_H
#define ITEM_H

#define MAX_NAME 50
#define INITIAL_CAPACITY 10
#define MAX_STOCK 100000
#define MAX_PRICE 10000000
#define ITEMS_FILE "data/items.txt"

typedef struct {
    int id;
    char name[MAX_NAME];
    char category[MAX_NAME];
    int stock;
    int price;
} Item;

typedef struct {
    Item *data;
    int count;
    int capacity;
} ItemList;

void initItemList(ItemList *list);
void freeItemList(ItemList *list);
int loadItems(ItemList *list);
int saveItems(const ItemList *list);
int addItem(ItemList *list);
void displayItems(const ItemList *list);
int updateStock(ItemList *list);
int deleteItem(ItemList *list);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "utils.h"

void initItemList(ItemList *list) {
    list->data = (Item *)malloc(INITIAL_CAPACITY * sizeof(Item));
    list->count = 0;
    list->capacity = (list->data != NULL) ? INITIAL_CAPACITY : 0;
}

void freeItemList(ItemList *list) {
    free(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int ensureCapacity(ItemList *list) {
    Item *resized;
    int newCapacity;

    if (list->count < list->capacity) {
        return 1;
    }
    newCapacity = (list->capacity == 0) ? INITIAL_CAPACITY : list->capacity * 2;
    resized = (Item *)realloc(list->data, newCapacity * sizeof(Item));
    if (resized == NULL) {
        return 0;
    }
    list->data = resized;
    list->capacity = newCapacity;
    return 1;
}

static int nextItemId(const ItemList *list) {
    int maxId = 0;
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->data[i].id > maxId) {
            maxId = list->data[i].id;
        }
    }
    return maxId + 1;
}

int findIndexById(const ItemList *list, int id) {
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->data[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int isValidText(const char *text) {
    int length = (int)strlen(text);
    return length > 0 && length < MAX_NAME && strchr(text, '|') == NULL;
}

int loadItems(ItemList *list) {
    char line[MAX_INPUT * 2];
    Item item;
    FILE *file = fopen(ITEMS_FILE, "r");

    if (file == NULL) {
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%49[^|]|%49[^|]|%d|%d",
                   &item.id, item.name, item.category, &item.stock, &item.price) == 5) {
            if (!ensureCapacity(list)) {
                fclose(file);
                return 0;
            }
            list->data[list->count++] = item;
        }
    }
    fclose(file);
    return 1;
}

int saveItems(const ItemList *list) {
    int i;
    FILE *file = fopen(ITEMS_FILE, "w");

    if (file == NULL) {
        printf("Gagal menyimpan data ke file.\n");
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        fprintf(file, "%d|%s|%s|%d|%d\n", list->data[i].id, list->data[i].name,
                list->data[i].category, list->data[i].stock, list->data[i].price);
    }
    fclose(file);
    return 1;
}

int addItem(ItemList *list) {
    char name[MAX_INPUT];
    char category[MAX_INPUT];
    Item item;

    readLine("Nama item: ", name, sizeof(name));
    if (!isValidText(name)) {
        printf("Nama tidak valid (kosong, terlalu panjang, atau memuat '|').\n");
        return 0;
    }
    readLine("Kategori: ", category, sizeof(category));
    if (!isValidText(category)) {
        printf("Kategori tidak valid (kosong, terlalu panjang, atau memuat '|').\n");
        return 0;
    }
    item.stock = readInt("Stok: ", 0, MAX_STOCK);
    item.price = readInt("Harga: ", 0, MAX_PRICE);

    if (!ensureCapacity(list)) {
        printf("Memori tidak cukup.\n");
        return 0;
    }
    item.id = nextItemId(list);
    strcpy(item.name, name);
    strcpy(item.category, category);
    list->data[list->count++] = item;
    printf("Item berhasil ditambahkan dengan ID %d.\n", item.id);
    return 1;
}

void displayItems(const ItemList *list) {
    int i;

    if (list->count == 0) {
        printf("Belum ada item.\n");
        return;
    }
    printf("\n%-4s %-20s %-14s %8s %10s\n", "ID", "Nama", "Kategori", "Stok", "Harga");
    for (i = 0; i < list->count; i++) {
        printf("%-4d %-20s %-14s %8d %10d\n", list->data[i].id, list->data[i].name,
               list->data[i].category, list->data[i].stock, list->data[i].price);
    }
}

int updateStock(ItemList *list) {
    int id = readInt("ID item: ", 1, 1000000);
    int index = findIndexById(list, id);

    if (index < 0) {
        printf("Item dengan ID %d tidak ditemukan.\n", id);
        return 0;
    }
    list->data[index].stock = readInt("Stok baru: ", 0, MAX_STOCK);
    printf("Stok %s diperbarui.\n", list->data[index].name);
    return 1;
}

int deleteItem(ItemList *list) {
    int id = readInt("ID item: ", 1, 1000000);
    int index = findIndexById(list, id);
    int i;

    if (index < 0) {
        printf("Item dengan ID %d tidak ditemukan.\n", id);
        return 0;
    }
    for (i = index; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->count--;
    printf("Item berhasil dihapus.\n");
    return 1;
}

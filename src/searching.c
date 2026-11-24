#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "searching.h"
#include "sorting.h"
#include "utils.h"

static int containsIgnoreCase(const char *text, const char *keyword) {
    int textLength = (int)strlen(text);
    int keywordLength = (int)strlen(keyword);
    int i, j;

    if (keywordLength == 0) {
        return 0;
    }
    for (i = 0; i + keywordLength <= textLength; i++) {
        for (j = 0; j < keywordLength; j++) {
            if (tolower((unsigned char)text[i + j]) != tolower((unsigned char)keyword[j])) {
                break;
            }
        }
        if (j == keywordLength) {
            return 1;
        }
    }
    return 0;
}

static void printItemRow(const Item *item) {
    printf("%-4d %-20s %-14s %8d %10d\n", item->id, item->name,
           item->category, item->stock, item->price);
}

static void printItemHeader(void) {
    printf("\n%-4s %-20s %-14s %8s %10s\n", "ID", "Nama", "Kategori", "Stok", "Harga");
}

int sequentialSearchByName(const ItemList *list, const char *keyword) {
    int i;
    int found = 0;

    for (i = 0; i < list->count; i++) {
        if (containsIgnoreCase(list->data[i].name, keyword)) {
            if (found == 0) {
                printItemHeader();
            }
            printItemRow(&list->data[i]);
            found++;
        }
    }
    return found;
}

int binarySearchById(const ItemList *list, int id) {
    int low = 0;
    int high = list->count - 1;
    int mid;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (list->data[mid].id == id) {
            return mid;
        }
        if (list->data[mid].id < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void runSequentialSearch(const ItemList *list) {
    char keyword[MAX_INPUT];
    int found;

    readLine("Kata kunci nama: ", keyword, sizeof(keyword));
    if (strlen(keyword) == 0) {
        printf("Kata kunci tidak boleh kosong.\n");
        return;
    }
    found = sequentialSearchByName(list, keyword);
    if (found == 0) {
        printf("Item dengan nama \"%s\" tidak ditemukan.\n", keyword);
    } else {
        printf("Ditemukan %d item (sequential search).\n", found);
    }
}

void runBinarySearch(ItemList *list) {
    int id, index;

    if (list->count == 0) {
        printf("Belum ada item.\n");
        return;
    }
    if (!mergeSortItems(list->data, list->count, SORT_BY_ID, 1)) {
        printf("Memori tidak cukup untuk mengurutkan data.\n");
        return;
    }
    printf("Data diurutkan berdasarkan ID terlebih dahulu (syarat binary search).\n");
    id = readInt("ID yang dicari: ", 1, 1000000);
    index = binarySearchById(list, id);
    if (index < 0) {
        printf("Item dengan ID %d tidak ditemukan.\n", id);
        return;
    }
    printItemHeader();
    printItemRow(&list->data[index]);
    printf("Ditemukan pada indeks %d (binary search).\n", index);
}

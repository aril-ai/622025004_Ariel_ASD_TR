#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"
#include "utils.h"

static int compareItems(const Item *a, const Item *b, int field, int ascending) {
    int result = 0;

    switch (field) {
        case SORT_BY_ID:    result = a->id - b->id; break;
        case SORT_BY_NAME:  result = strcmp(a->name, b->name); break;
        case SORT_BY_STOCK: result = a->stock - b->stock; break;
        case SORT_BY_PRICE: result = a->price - b->price; break;
        default: break;
    }
    return ascending ? result : -result;
}

void bubbleSortItems(Item *data, int count, int field, int ascending) {
    int i, j, swapped;
    Item temp;

    for (i = 0; i < count - 1; i++) {
        swapped = 0;
        for (j = 0; j < count - i - 1; j++) {
            if (compareItems(&data[j], &data[j + 1], field, ascending) > 0) {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

static void mergeRanges(Item *data, Item *temp, int left, int mid, int right,
                        int field, int ascending) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (compareItems(&data[i], &data[j], field, ascending) <= 0) {
            temp[k++] = data[i++];
        } else {
            temp[k++] = data[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = data[i++];
    }
    while (j <= right) {
        temp[k++] = data[j++];
    }
    for (k = left; k <= right; k++) {
        data[k] = temp[k];
    }
}

static void mergeSortRange(Item *data, Item *temp, int left, int right,
                           int field, int ascending) {
    int mid;

    if (left >= right) {
        return;
    }
    mid = left + (right - left) / 2;
    mergeSortRange(data, temp, left, mid, field, ascending);
    mergeSortRange(data, temp, mid + 1, right, field, ascending);
    mergeRanges(data, temp, left, mid, right, field, ascending);
}

int mergeSortItems(Item *data, int count, int field, int ascending) {
    Item *temp;

    if (count < 2) {
        return 1;
    }
    temp = (Item *)malloc(count * sizeof(Item));
    if (temp == NULL) {
        return 0;
    }
    mergeSortRange(data, temp, 0, count - 1, field, ascending);
    free(temp);
    return 1;
}

void runSortMenu(ItemList *list) {
    int algorithm, field, order, ascending;

    if (list->count < 2) {
        printf("Data kurang dari 2 item, tidak perlu diurutkan.\n");
        return;
    }
    printf("\nAlgoritma: 1. Bubble Sort  2. Merge Sort\n");
    algorithm = readInt("Pilih algoritma: ", 1, 2);
    printf("Urutkan berdasarkan: 1. ID  2. Nama  3. Stok  4. Harga\n");
    field = readInt("Pilih: ", 1, 4);
    printf("Urutan: 1. Ascending  2. Descending\n");
    order = readInt("Pilih: ", 1, 2);
    ascending = (order == 1);

    if (algorithm == 1) {
        bubbleSortItems(list->data, list->count, field, ascending);
    } else if (!mergeSortItems(list->data, list->count, field, ascending)) {
        printf("Memori tidak cukup untuk Merge Sort.\n");
        return;
    }
    printf("Data berhasil diurutkan.\n");
    displayItems(list);
}

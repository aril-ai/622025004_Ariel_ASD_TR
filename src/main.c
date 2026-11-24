#include <stdio.h>
#include "auth.h"
#include "item.h"
#include "sorting.h"
#include "searching.h"
#include "hashtable.h"
#include "utils.h"

static void showMainMenu(const char *username, ItemList *items) {
    int choice;
    do {
        printf("\n=== MENU UTAMA (%s) ===\n", username);
        printf("1. Tambah item\n2. Lihat semua item\n3. Ubah stok\n4. Hapus item\n");
        printf("5. Urutkan item\n6. Cari item (nama, sequential)\n");
        printf("7. Cari item (ID, binary)\n8. Cari item (ID, hash table)\n0. Logout\n");
        choice = readInt("Pilih: ", 0, 8);
        switch (choice) {
            case 1: if (addItem(items)) { saveItems(items); } break;
            case 2: displayItems(items); break;
            case 3: if (updateStock(items)) { saveItems(items); } break;
            case 4: if (deleteItem(items)) { saveItems(items); } break;
            case 5: runSortMenu(items); break;
            case 6: runSequentialSearch(items); break;
            case 7: runBinarySearch(items); break;
            case 8: runHashSearch(items); break;
            default: break;
        }
    } while (choice != 0);
    printf("Logout berhasil.\n");
}

int main(void) {
    int choice;
    char loggedInUser[MAX_USERNAME];
    ItemList items;

    initItemList(&items);
    if (items.data == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        return 1;
    }
    if (!loadItems(&items)) {
        printf("Peringatan: data item tidak terbaca sepenuhnya.\n");
    }

    do {
        printf("\n=== KAFE INVENTORI ===\n1. Register\n2. Login\n0. Keluar\n");
        choice = readInt("Pilih: ", 0, 2);
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            if (loginUser(loggedInUser, sizeof(loggedInUser))) {
                showMainMenu(loggedInUser, &items);
            }
        }
    } while (choice != 0);

    freeItemList(&items);
    return 0;
}

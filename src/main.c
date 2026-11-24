#include <stdio.h>
#include "auth.h"
#include "item.h"
#include "sorting.h"
#include "searching.h"
#include "hashtable.h"
#include "stack.h"
#include "queue.h"
#include "utils.h"

static void showMainMenu(const char *username, ItemList *items,
                         UndoStack *history, OrderQueue *orders) {
    int choice;
    do {
        printf("\n=== MENU UTAMA (%s) ===\n", username);
        printf("-- Data --\n1. Tambah item\n2. Lihat semua item\n");
        printf("3. Ubah stok\n4. Hapus item\n");
        printf("-- Urutkan & Cari --\n5. Urutkan item\n6. Cari nama (sequential)\n");
        printf("7. Cari ID (binary)\n8. Cari ID (hash table)\n");
        printf("-- Undo & Pesanan --\n9. Undo perubahan stok\n10. Lihat riwayat undo\n");
        printf("11. Tambah pesanan ke antrean\n12. Proses pesanan berikutnya\n");
        printf("13. Lihat antrean pesanan\n0. Logout\n");
        choice = readInt("Pilih: ", 0, 13);
        switch (choice) {
            case 1: if (addItem(items)) { saveItems(items); } break;
            case 2: displayItems(items); break;
            case 3: if (changeStockWithUndo(items, history)) { saveItems(items); } break;
            case 4: if (deleteItem(items)) { saveItems(items); } break;
            case 5: runSortMenu(items); break;
            case 6: runSequentialSearch(items); break;
            case 7: runBinarySearch(items); break;
            case 8: runHashSearch(items); break;
            case 9: if (undoStockChange(items, history)) { saveItems(items); } break;
            case 10: showUndoHistory(history); break;
            case 11: addOrder(items, orders); break;
            case 12: if (processNextOrder(items, orders)) { saveItems(items); } break;
            case 13: showOrderQueue(orders); break;
            default: break;
        }
    } while (choice != 0);
    printf("Logout berhasil.\n");
}

int main(void) {
    int choice;
    char loggedInUser[MAX_USERNAME];
    ItemList items;
    UndoStack history;
    OrderQueue orders;

    initItemList(&items);
    if (items.data == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        return 1;
    }
    initStack(&history);
    initQueue(&orders);
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
                showMainMenu(loggedInUser, &items, &history, &orders);
            }
        }
    } while (choice != 0);

    clearStack(&history);
    clearQueue(&orders);
    freeItemList(&items);
    return 0;
}

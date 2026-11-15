#include <stdio.h>
#include "utils.h"

int main(void) {
    int choice;
    do {
        printf("\n=== KAFE INVENTORI ===\n1. Tes\n0. Keluar\n");
        choice = readInt("Pilih: ", 0, 1);
    } while (choice != 0);
    return 0;
}

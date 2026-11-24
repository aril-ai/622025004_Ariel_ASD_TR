#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "utils.h"

void initStack(UndoStack *stack) {
    stack->top = NULL;
    stack->size = 0;
}

int pushStack(UndoStack *stack, int itemId, int oldStock) {
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));

    if (node == NULL) {
        return 0;
    }
    node->itemId = itemId;
    node->oldStock = oldStock;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    return 1;
}

int popStack(UndoStack *stack, int *itemId, int *oldStock) {
    StackNode *node;

    if (stack->top == NULL) {
        return 0;
    }
    node = stack->top;
    *itemId = node->itemId;
    *oldStock = node->oldStock;
    stack->top = node->next;
    free(node);
    stack->size--;
    return 1;
}

int isStackEmpty(const UndoStack *stack) {
    return stack->top == NULL;
}

void clearStack(UndoStack *stack) {
    int itemId, oldStock;

    while (popStack(stack, &itemId, &oldStock)) {
    }
}

int changeStockWithUndo(ItemList *list, UndoStack *stack) {
    int id = readInt("ID item: ", 1, 1000000);
    int index = findIndexById(list, id);
    int newStock;

    if (index < 0) {
        printf("Item dengan ID %d tidak ditemukan.\n", id);
        return 0;
    }
    newStock = readInt("Stok baru: ", 0, MAX_STOCK);
    if (!pushStack(stack, id, list->data[index].stock)) {
        printf("Memori tidak cukup untuk riwayat undo.\n");
        return 0;
    }
    list->data[index].stock = newStock;
    printf("Stok %s diubah menjadi %d.\n", list->data[index].name, newStock);
    return 1;
}

int undoStockChange(ItemList *list, UndoStack *stack) {
    int id, oldStock, index;

    if (!popStack(stack, &id, &oldStock)) {
        printf("Tidak ada perubahan stok yang bisa di-undo.\n");
        return 0;
    }
    index = findIndexById(list, id);
    if (index < 0) {
        printf("Item ID %d sudah dihapus, undo dibatalkan.\n", id);
        return 0;
    }
    list->data[index].stock = oldStock;
    printf("Undo berhasil: stok %s dikembalikan ke %d.\n", list->data[index].name, oldStock);
    return 1;
}

void showUndoHistory(const UndoStack *stack) {
    const StackNode *current = stack->top;
    int number = 1;

    if (current == NULL) {
        printf("Riwayat perubahan stok kosong.\n");
        return;
    }
    printf("Riwayat perubahan stok (terbaru di atas), total %d:\n", stack->size);
    while (current != NULL) {
        printf("%d. Item ID %d (stok sebelumnya: %d)\n", number, current->itemId, current->oldStock);
        current = current->next;
        number++;
    }
}

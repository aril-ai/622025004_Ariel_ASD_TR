#ifndef STACK_H
#define STACK_H

#include "item.h"

typedef struct StackNode {
    int itemId;
    int oldStock;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} UndoStack;

void initStack(UndoStack *stack);
int pushStack(UndoStack *stack, int itemId, int oldStock);
int popStack(UndoStack *stack, int *itemId, int *oldStock);
int isStackEmpty(const UndoStack *stack);
void clearStack(UndoStack *stack);
int changeStockWithUndo(ItemList *list, UndoStack *stack);
int undoStockChange(ItemList *list, UndoStack *stack);
void showUndoHistory(const UndoStack *stack);

#endif

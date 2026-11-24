#ifndef QUEUE_H
#define QUEUE_H

#include "item.h"

typedef struct OrderNode {
    int orderId;
    char customer[MAX_NAME];
    int itemId;
    int quantity;
    struct OrderNode *next;
} OrderNode;

typedef struct {
    OrderNode *front;
    OrderNode *rear;
    int size;
    int nextOrderId;
} OrderQueue;

void initQueue(OrderQueue *queue);
int enqueueOrder(OrderQueue *queue, const char *customer, int itemId, int quantity);
int dequeueOrder(OrderQueue *queue, OrderNode *result);
void clearQueue(OrderQueue *queue);
void addOrder(const ItemList *list, OrderQueue *queue);
int processNextOrder(ItemList *list, OrderQueue *queue);
void showOrderQueue(const OrderQueue *queue);

#endif

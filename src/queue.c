#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "utils.h"

void initQueue(OrderQueue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    queue->nextOrderId = 1;
}

int enqueueOrder(OrderQueue *queue, const char *customer, int itemId, int quantity) {
    OrderNode *node = (OrderNode *)malloc(sizeof(OrderNode));

    if (node == NULL) {
        return 0;
    }
    node->orderId = queue->nextOrderId++;
    strncpy(node->customer, customer, MAX_NAME - 1);
    node->customer[MAX_NAME - 1] = '\0';
    node->itemId = itemId;
    node->quantity = quantity;
    node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return node->orderId;
}

int dequeueOrder(OrderQueue *queue, OrderNode *result) {
    OrderNode *node;

    if (queue->front == NULL) {
        return 0;
    }
    node = queue->front;
    *result = *node;
    result->next = NULL;
    queue->front = node->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(node);
    queue->size--;
    return 1;
}

void clearQueue(OrderQueue *queue) {
    OrderNode discarded;

    while (dequeueOrder(queue, &discarded)) {
    }
}

void addOrder(const ItemList *list, OrderQueue *queue) {
    char customer[MAX_INPUT];
    int itemId, quantity, orderId;

    readLine("Nama pelanggan: ", customer, sizeof(customer));
    if (strlen(customer) == 0 || strlen(customer) >= MAX_NAME) {
        printf("Nama pelanggan tidak valid (kosong atau terlalu panjang).\n");
        return;
    }
    itemId = readInt("ID item dipesan: ", 1, 1000000);
    if (findIndexById(list, itemId) < 0) {
        printf("Item dengan ID %d tidak ditemukan.\n", itemId);
        return;
    }
    quantity = readInt("Jumlah: ", 1, MAX_STOCK);
    orderId = enqueueOrder(queue, customer, itemId, quantity);
    if (orderId == 0) {
        printf("Memori tidak cukup untuk antrean.\n");
        return;
    }
    printf("Pesanan #%d atas nama %s masuk antrean (posisi %d).\n", orderId, customer, queue->size);
}

int processNextOrder(ItemList *list, OrderQueue *queue) {
    OrderNode order;
    int index;

    if (!dequeueOrder(queue, &order)) {
        printf("Antrean kosong.\n");
        return 0;
    }
    index = findIndexById(list, order.itemId);
    if (index < 0) {
        printf("Pesanan #%d (%s) ditolak: item ID %d sudah tidak ada.\n",
               order.orderId, order.customer, order.itemId);
        return 0;
    }
    if (list->data[index].stock < order.quantity) {
        printf("Pesanan #%d (%s) ditolak: stok %s hanya %d, diminta %d.\n",
               order.orderId, order.customer, list->data[index].name,
               list->data[index].stock, order.quantity);
        return 0;
    }
    list->data[index].stock -= order.quantity;
    printf("Pesanan #%d (%s) diproses: %d x %s. Sisa stok %d.\n",
           order.orderId, order.customer, order.quantity,
           list->data[index].name, list->data[index].stock);
    return 1;
}

void showOrderQueue(const OrderQueue *queue) {
    const OrderNode *current = queue->front;
    int position = 1;

    if (current == NULL) {
        printf("Antrean pesanan kosong.\n");
        return;
    }
    printf("Antrean pesanan (%d menunggu):\n", queue->size);
    while (current != NULL) {
        printf("%d. Pesanan #%d | %s | item ID %d | jumlah %d\n",
               position, current->orderId, current->customer, current->itemId, current->quantity);
        current = current->next;
        position++;
    }
}

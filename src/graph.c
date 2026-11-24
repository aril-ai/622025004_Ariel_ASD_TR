#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

static const char *itemNameById(const ItemList *list, int id) {
    int index = findIndexById(list, id);

    return (index >= 0) ? list->data[index].name : "?";
}

int findVertex(const Graph *graph, int id) {
    int i;

    for (i = 0; i < graph->vertexCount; i++) {
        if (graph->ids[i] == id) {
            return i;
        }
    }
    return -1;
}

int addEdge(Graph *graph, int idA, int idB) {
    int a = findVertex(graph, idA);
    int b = findVertex(graph, idB);

    if (a < 0 || b < 0) {
        return EDGE_NOT_FOUND;
    }
    if (a == b) {
        return EDGE_SELF;
    }
    if (graph->matrix[a][b]) {
        return EDGE_EXISTS;
    }
    graph->matrix[a][b] = 1;
    graph->matrix[b][a] = 1;
    return EDGE_OK;
}

void buildGraph(Graph *graph, const ItemList *list) {
    int i, idA, idB;
    FILE *file;

    memset(graph->matrix, 0, sizeof(graph->matrix));
    graph->vertexCount = (list->count > MAX_VERTEX) ? MAX_VERTEX : list->count;
    if (list->count > MAX_VERTEX) {
        printf("Peringatan: graph hanya memuat %d item pertama.\n", MAX_VERTEX);
    }
    for (i = 0; i < graph->vertexCount; i++) {
        graph->ids[i] = list->data[i].id;
    }
    file = fopen(GRAPH_FILE, "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d %d", &idA, &idB) == 2) {
        addEdge(graph, idA, idB);
    }
    fclose(file);
}

static int appendEdgeToFile(int idA, int idB) {
    FILE *file = fopen(GRAPH_FILE, "a");

    if (file == NULL) {
        printf("Gagal menyimpan relasi ke file.\n");
        return 0;
    }
    fprintf(file, "%d %d\n", idA, idB);
    fclose(file);
    return 1;
}

void bfsTraversal(const Graph *graph, const ItemList *list, int startId) {
    int visited[MAX_VERTEX] = {0};
    int level[MAX_VERTEX] = {0};
    int queue[MAX_VERTEX];
    int front = 0;
    int rear = 0;
    int lastLevel = -1;
    int start, current, next;

    start = findVertex(graph, startId);
    if (start < 0) {
        printf("Item ID %d tidak ada di graph.\n", startId);
        return;
    }
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS dari %s (item terdekat dijangkau lebih dulu):", itemNameById(list, startId));
    while (front < rear) {
        current = queue[front++];
        if (level[current] != lastLevel) {
            lastLevel = level[current];
            printf("\n  Jarak %d: ", lastLevel);
        }
        printf("%s  ", itemNameById(list, graph->ids[current]));
        for (next = 0; next < graph->vertexCount; next++) {
            if (graph->matrix[current][next] && !visited[next]) {
                visited[next] = 1;
                level[next] = level[current] + 1;
                queue[rear++] = next;
            }
        }
    }
    printf("\nTotal item terjangkau (termasuk titik awal): %d dari %d\n", rear, graph->vertexCount);
}

static void dfsVisit(const Graph *graph, const ItemList *list, int vertex,
                     int *visited, int depth) {
    int next;

    visited[vertex] = 1;
    printf("%*s- %s\n", depth * 2 + 2, "", itemNameById(list, graph->ids[vertex]));
    for (next = 0; next < graph->vertexCount; next++) {
        if (graph->matrix[vertex][next] && !visited[next]) {
            dfsVisit(graph, list, next, visited, depth + 1);
        }
    }
}

void dfsTraversal(const Graph *graph, const ItemList *list, int startId) {
    int visited[MAX_VERTEX] = {0};
    int start = findVertex(graph, startId);

    if (start < 0) {
        printf("Item ID %d tidak ada di graph.\n", startId);
        return;
    }
    printf("DFS dari %s (rantai relasi, makin menjorok = makin jauh):\n",
           itemNameById(list, startId));
    dfsVisit(graph, list, start, visited, 0);
}

static void showRelations(const Graph *graph, const ItemList *list) {
    int i, j, hasEdge, anyRelation = 0;

    for (i = 0; i < graph->vertexCount; i++) {
        hasEdge = 0;
        for (j = 0; j < graph->vertexCount; j++) {
            if (graph->matrix[i][j]) {
                if (!hasEdge) {
                    printf("%s (ID %d) berelasi dengan: ", itemNameById(list, graph->ids[i]), graph->ids[i]);
                    hasEdge = 1;
                    anyRelation = 1;
                }
                printf("%s  ", itemNameById(list, graph->ids[j]));
            }
        }
        if (hasEdge) {
            printf("\n");
        }
    }
    if (!anyRelation) {
        printf("Belum ada relasi antar item.\n");
    }
}

void runGraphMenu(const ItemList *list) {
    Graph graph;
    int choice, idA, idB, result;

    if (list->count == 0) {
        printf("Belum ada item, graph tidak bisa dibuat.\n");
        return;
    }
    buildGraph(&graph, list);
    do {
        printf("\n--- GRAPH RELASI ITEM (sering dipadukan dalam satu menu) ---\n");
        printf("1. Tambah relasi\n2. Lihat semua relasi\n3. BFS dari item\n4. DFS dari item\n0. Kembali\n");
        choice = readInt("Pilih: ", 0, 4);
        switch (choice) {
            case 1:
                idA = readInt("ID item pertama: ", 1, 1000000);
                idB = readInt("ID item kedua: ", 1, 1000000);
                result = addEdge(&graph, idA, idB);
                if (result == EDGE_OK) {
                    if (appendEdgeToFile(idA, idB)) {
                        printf("Relasi %s <-> %s ditambahkan.\n",
                               itemNameById(list, idA), itemNameById(list, idB));
                    }
                } else if (result == EDGE_NOT_FOUND) {
                    printf("Salah satu ID item tidak ditemukan.\n");
                } else if (result == EDGE_SELF) {
                    printf("Item tidak bisa berelasi dengan dirinya sendiri.\n");
                } else {
                    printf("Relasi tersebut sudah ada.\n");
                }
                break;
            case 2:
                showRelations(&graph, list);
                break;
            case 3:
                bfsTraversal(&graph, list, readInt("ID item awal: ", 1, 1000000));
                break;
            case 4:
                dfsTraversal(&graph, list, readInt("ID item awal: ", 1, 1000000));
                break;
            default:
                break;
        }
    } while (choice != 0);
}

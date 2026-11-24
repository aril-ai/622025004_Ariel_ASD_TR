#ifndef GRAPH_H
#define GRAPH_H

#include "item.h"

#define MAX_VERTEX 100
#define GRAPH_FILE "data/graph.txt"

#define EDGE_NOT_FOUND 0
#define EDGE_OK 1
#define EDGE_SELF 2
#define EDGE_EXISTS 3

typedef struct {
    int ids[MAX_VERTEX];
    int vertexCount;
    int matrix[MAX_VERTEX][MAX_VERTEX];
} Graph;

void buildGraph(Graph *graph, const ItemList *list);
int findVertex(const Graph *graph, int id);
int addEdge(Graph *graph, int idA, int idB);
void bfsTraversal(const Graph *graph, const ItemList *list, int startId);
void dfsTraversal(const Graph *graph, const ItemList *list, int startId);
void runGraphMenu(const ItemList *list);

#endif

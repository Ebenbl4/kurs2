#pragma once
#include <stdbool.h>

typedef struct {
    int numVertices;
    int** adjMatrix;
} Graph;
typedef struct Node {
    int u, v;
    struct Node* next;
} Node;
typedef struct EdgeSetList {
    Node* edgeSet;
    struct EdgeSetList* next;
} EdgeSetList;
bool writeToFile(Graph** graph, EdgeSetList* list);
bool readFromFile(Graph** graph);


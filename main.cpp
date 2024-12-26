#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>
#include <stdbool.h>
#include "io.h"

void freeEdgeSetList(EdgeSetList* list);
Graph* copyGraph(Graph* original);
bool graphOrientedCheck(Graph* graph);
void addEdgeSetToList(EdgeSetList** list, Node* edgeSet);
void printAllEdgeSets(EdgeSetList* list);
Node* createNode(int u, int v);
void freeEdgeSet(Node* edgeSet);
void addEdgeOriented(Graph* graph, int u, int v);
int initializeGraph(Graph* graph, int probability);
void addEdge(Graph* graph, int u, int v);
bool isIndependentSet(Node* edgeSet);
void printIndependentEdges(Node* edgeSet);
void removeEdge(Graph* graph, int u, int v, bool oriented);
void greedyAlgIterable(Graph* graph, EdgeSetList** results, bool oriented);
void greedyAlg(Graph* graph, Node** edgeSet);
int initializeGraphOriented(Graph* graph, int probability);
void printGraph(int** graph_pointer, int size);
void askFileWrite(Graph* graph, EdgeSetList* list);
Graph* createGraph(int numVertices);
void freeGraph(Graph* graph);

void freeEdgeSetList(EdgeSetList* list) {
    while (list != NULL) {
        EdgeSetList* temp = list;
        list = list->next;
        freeEdgeSet(temp->edgeSet); 
        free(temp); 
    }
}

Graph* copyGraph(Graph* original) {
    Graph* newGraph = createGraph(original->numVertices);
    for (int i = 0; i < original->numVertices; i++) {
        for (int j = 0; j < original->numVertices; j++) {
            newGraph->adjMatrix[i][j] = original->adjMatrix[i][j];
        }
    }
    return newGraph;
}

bool graphOrientedCheck(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = i + 1; j < graph->numVertices; j++) {
            if (graph->adjMatrix[i][j] != graph->adjMatrix[j][i]) {
                return true; 
            }
        }
    }
    return false;
}

void addEdgeSetToList(EdgeSetList** list, Node* edgeSet) {
    EdgeSetList* newNode = (EdgeSetList*)malloc(sizeof(EdgeSetList));
    newNode->edgeSet = edgeSet;
    newNode->next = *list;
    *list = newNode;
}

void printAllEdgeSets(EdgeSetList* list) {
    if (list == NULL) {
        printf("Список множеств независимых рёбер пуст.\n");
        return;
    }
    printf("Все множества независимых рёбер:\n");
        int iteration = 1;
    while (list != NULL) {
        printf("Итерация %d:\n", iteration++);
            printIndependentEdges(list->edgeSet);
        list = list->next;
    }
}

Node* createNode(int u, int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->u = u;
    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

void freeEdgeSet(Node* edgeSet) {
    Node* current = edgeSet;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}
void addEdgeOriented(Graph* graph, int u, int v) {
    graph->adjMatrix[u][v] = 1;
    graph->adjMatrix[v][u] = 1;
}

int initializeGraph(Graph* graph, int probability) {
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            if (rand() % 100 < probability && i != j && graph->adjMatrix[i][j] == 0 && graph->adjMatrix[j][i] == 0) {
                addEdge(graph, i, j);
            }
        }
    }
    return 0;
}

Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjMatrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjMatrix[i] = (int*)calloc(numVertices, sizeof(int));
    }
    return graph;
}

void addEdge(Graph* graph, int u, int v) {
    graph->adjMatrix[u][v] = 1;
}

void printIndependentEdges(Node* edgeSet) {
    Node* current = edgeSet;
    while (current != NULL) {
        printf("(%d, %d) ", current->u + 1, current->v + 1);
        current = current->next;
    }
    printf("\n");
}

void removeEdge(Graph* graph, int u, int v, bool oriented) {
    graph->adjMatrix[u][v] = 0;
    if (!oriented) {
        graph->adjMatrix[v][u] = 0; 
    }
}

bool isIndependentSet(Node* edgeSet) {
    Node* current = edgeSet;
    while (current != NULL) {
        Node* check = current->next;
        while (check != NULL) {
            if (current->u == check->u || current->u == check->v || current->v == check->u || current->v == check->v) {
                return false;
            }
            check = check->next;
        }
        current = current->next;
    }
    return true;
}

void greedyAlgIterable(Graph* graph, EdgeSetList** results, bool oriented) {
    while (true) {
        Node* edgeSet = NULL;
        greedyAlg(graph, &edgeSet);
        if (edgeSet == NULL) break;
        addEdgeSetToList(results, edgeSet);
        Node* current = edgeSet;
        while (current != NULL) {
            int u = current->u;
            int v = current->v;
            removeEdge(graph, u, v, oriented);
            current = current->next;
        }
    }
}


void greedyAlg(Graph* graph, Node** edgeSet) {
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->adjMatrix[i][j] == 1) {
                Node* tempEdge = (Node*)malloc(sizeof(Node));
                tempEdge->u = i;
                tempEdge->v = j;
                tempEdge->next = *edgeSet;
                *edgeSet = tempEdge;
                if (!isIndependentSet(*edgeSet)) {
                    *edgeSet = (*edgeSet)->next;
                    free(tempEdge);
                }
            }
        }
    }
}

int initializeGraphOriented(Graph* graph, int probability) {
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = i+1; j < graph->numVertices; j++) {
            if (rand() % 100 < probability && i != j && graph->adjMatrix[i][j] == 0) {
                addEdgeOriented(graph, i, j);
            }
        }
    }
    return 0;
}

void printGraph(int** graph_pointer, int size) {
    printf(" ");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(stdout, "%d ", graph_pointer[i][j]);
        }
        fprintf(stdout, "\n ");
    }
}

void freeGraph(Graph* graph) {
    if (graph != NULL) {
        for (int i = 0; i < graph->numVertices; i++) {
            free(graph->adjMatrix[i]);
        }
        free(graph->adjMatrix);
        free(graph);
    }
}

void askFileWrite(Graph* graph,EdgeSetList* list) {
    int choice = -1;
    while (1) {
        printf("\nСохранить файл? 1 - Да, 0 - Нет\nВвод: ");
        if (!scanf("%d", &choice)) {
            printf("Ошибка ввода\n");
            continue;
        }
        switch (choice)
        {
        case 1:
            if(!writeToFile(&graph, list)) break;
            return;
        case 0:
            return;
        default:
            printf("Ошибка ввода\n");
            break;
        }

    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));
    Graph* graph = NULL;
    Graph* graph_copy = NULL;
    EdgeSetList* set = NULL;
    Node* edgeSet = NULL;
    int choice = -1;
    FILE* file = NULL;
    bool oriented = false;
    int graph_size = 0;
    bool graph_type_cycle = true;
    int graph_type;
    int probability;
    bool err = false;
    while (1) {
        graph_type_cycle = true;
        printf("1. Чтение графа из файла \n");
        printf("2. Генерация случайного графа \n");
        printf("3. Выход\n");
        printf("Ввод: ");
        choice = -1;
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            if (readFromFile(&graph) == false) break;
            oriented = graphOrientedCheck(graph);
            graph_copy = copyGraph(graph);
            printGraph(graph_copy->adjMatrix, graph_copy->numVertices);
            printf("\n");
            greedyAlgIterable(graph_copy, &set, oriented);
            printAllEdgeSets(set);
            askFileWrite(graph, set);
            freeEdgeSetList(set);
            set = NULL;
            freeGraph(graph);
            graph = NULL;
            freeGraph(graph_copy);
            graph_copy = NULL;
            break;
        case 2:
            printf("Выберите размер графа: ");
            if (!scanf("%d", &graph_size) || graph_size <= 1) {
                printf("Ошибка ввода количества вершин\n");
                break;
            }
            printf("Выберите вероятность появления ребра: ");
            if (!scanf("%d", &probability) || probability <= 0 || probability > 100) {
                printf("Ошибка ввода вероятности\n");
                break;
            }
            graph = createGraph(graph_size);
            getchar();
            while (graph_type_cycle) {
                err = false;
                printf("Какой граф будет сгенерирован? Ориентированный? 1 - Да, 0 - Нет\nВвод: ");
                if (!scanf("%d", &graph_type)) {
                    printf("Ошибка ввода");
                    continue;
                };
                switch (graph_type)
                {
                case 0:
                    initializeGraphOriented(graph, probability);
                    graph_type_cycle = false;
                    oriented = false;
                    break;
                case 1:
                    initializeGraph(graph, probability);
                    graph_type_cycle = false;
                    oriented = true;
                    break;
                default:
                    printf("Ошибка ввода\n");
                    err = true;
                    break;
                }
                if (err == true) continue;
                graph_copy = copyGraph(graph);
                printGraph(graph_copy->adjMatrix, graph_copy->numVertices);
                printf("\n");
                greedyAlgIterable(graph_copy, &set, oriented);
                printAllEdgeSets(set);
                askFileWrite(graph, set);
            }
            freeEdgeSetList(set);
            set = NULL;
            freeGraph(graph);
            graph = NULL;
            freeGraph(graph_copy);
            graph_copy = NULL;
            break;
        case 3:
            exit(0);
        default:
            printf("Ошибка ввода\n");
            break;
        }
    }
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
Graph* createGraph(int numVertices);
void freeGraph(Graph* graph);





bool readFromFile(Graph** graph) {
	char file_name[127];
	printf("������� �������� ����� ��� ������ ������� ���������: ");
		if (!scanf("%127s", file_name)) {
			printf("������ ����� �������� �����");
			return false;
		}
	    FILE* file = fopen(file_name, "r");
	    if (file == NULL) {
			printf(" ������: ���� �� ���������� ��� �� ����� ���� ������\n");
			return false;
	    }
	    int size = 0;
	    char line[1024];
	    if (fgets(line, sizeof(line), file)) {
	        char* token = strtok(line, " \n");
	        while (token != NULL) {
	            size++;
	            token = strtok(NULL, " \n");
	        }
	    }
	    if (size < 2 || size > 1024) {
	        printf(" ������: ������������ ������ �������\n");
	        fclose(file);
	        return false;
	    }
	    rewind(file);
	    *graph = createGraph(size);
	    for (int i = 0; i < size; i++) {
	        for (int j = 0; j < size; j++) {
	            if (fscanf(file, "%hhd", &(*graph)->adjMatrix[i][j]) != 1) {
	                printf(" ������ ������ ������� ���������\n");
	                fclose(file);
					freeGraph(*graph);
	                return false;
	            }
	        }
	    }
	    fclose(file);
		return true;
}

bool writeToFile(Graph** graph, EdgeSetList* list) {
	char output_file_name[128];
	printf("\n������� �������� ����� ��� ���������� �����������: ");
	if (!scanf("%127s", output_file_name)) { 
		printf("������ ����� �������� �����");
		return false;
	}
	FILE* output_file = fopen(output_file_name, "w");
	if (output_file == NULL) {
		printf(" ������ �������� ��������� �����\n");
		return false;
	}
	for (int i = 0; i < (*graph)->numVertices; i++) {
		for (int j = 0; j < (*graph)->numVertices; j++) {
			fprintf(output_file, "%d ", (*graph)->adjMatrix[j][i]);
		}
		fprintf(output_file, "\n");
	}
	fprintf(output_file,"\n��� ��������� ����������� ����:\n");
	int iteration = 1;
	while (list != NULL) {
		fprintf(output_file, "�������� %d:\n", iteration++);
		{
			Node* current = list->edgeSet;
			while (current != NULL) {
				fprintf(output_file, "(%d, %d) ", current->u + 1, current->v + 1);
				current = current->next;
			}
			fprintf(output_file, "\n");
			list = list->next;
		}
	}
	fclose(output_file);
	return true;
}
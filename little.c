#include "little.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Copie une matrice de double
double** copy_matrix(double** matrix, int size) {
    double** new_matrix = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        new_matrix[i] = (double*)malloc(size * sizeof(double));
        for (int j = 0; j < size; j++) {
            new_matrix[i][j] = matrix[i][j];
        }
    }
    return new_matrix;
}

// Libère une matrice
void free_matrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Affiche une matrice
void print_matrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == INT_MAX) {
                printf("INF ");
            } else {
                printf("%5.1f ", matrix[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Réduction ligne + colonne, retourne coût de réduction
int reduce_matrix(double** matrix, int size) {
    int cost = 0;

    // Réduction ligne
    for (int i = 0; i < size; i++) {
        double min = INT_MAX;
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] < min) min = matrix[i][j];
        }
        if (min != INT_MAX && min != 0) {
            for (int j = 0; j < size; j++) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= min;
            }
            cost += min;
        }
    }

    // Réduction colonne
    for (int j = 0; j < size; j++) {
        double min = INT_MAX;
        for (int i = 0; i < size; i++) {
            if (matrix[i][j] < min) min = matrix[i][j];
        }
        if (min != INT_MAX && min != 0) {
            for (int i = 0; i < size; i++) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= min;
            }
            cost += min;
        }
    }

    return cost;
}

// Crée un noeud TSPNode
TSPNode* create_node(double** matrix, int size, int* path, int path_length, int lower_bound, int level) {
    TSPNode* node = (TSPNode*)malloc(sizeof(TSPNode));
    node->matrix = copy_matrix(matrix, size);
    node->size = size;
    node->path = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < path_length; i++) {
        node->path[i] = path[i];
    }
    node->path_length = path_length;
    node->lower_bound = lower_bound;
    node->level = level;
    return node;
}

int compare_nodes(const void* a, const void* b) {
    TSPNode* na = *(TSPNode**)a;
    TSPNode* nb = *(TSPNode**)b;
    return na->lower_bound - nb->lower_bound;
}

PriorityQueue* create_queue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->nodes = (TSPNode**)malloc(capacity * sizeof(TSPNode*));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void push(PriorityQueue* pq, TSPNode* node) {
    pq->nodes[pq->size++] = node;
    qsort(pq->nodes, pq->size, sizeof(TSPNode*), compare_nodes);
}

TSPNode* pop(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    return pq->nodes[--pq->size];
}

void free_queue(PriorityQueue* pq) {
    free(pq->nodes);
    free(pq);
}
// Meillure pénalité
void find_best_zero(double** matrix, int size, int* best_i, int* best_j) {
    int max_penalty = -1;
    *best_i = -1;
    *best_j = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == 0) {
                int row_min = INT_MAX;
                int col_min = INT_MAX;

                for (int k = 0; k < size; k++) {
                    if (k != j && matrix[i][k] < row_min) row_min = matrix[i][k];
                    if (k != i && matrix[k][j] < col_min) col_min = matrix[k][j];
                }

                int penalty = (row_min == INT_MAX ? 0 : row_min) + (col_min == INT_MAX ? 0 : col_min);

                if (penalty > max_penalty) {
                    max_penalty = penalty;
                    *best_i = i;
                    *best_j = j;
                }
            }
        }
    }
}


// Fonction principale (à implémenter ensuite)
void tsp_little(double** distance_matrix, int size) {
    int initial_path[1] = {0};
    int cost = reduce_matrix(distance_matrix, size);
    TSPNode* root = create_node(distance_matrix, size, initial_path, 1, cost, 0);

    PriorityQueue* pq = create_queue(100);
    push(pq, root);

    while (pq->size > 0) {
        TSPNode* current = pop(pq);
        printf("Exploration niveau %d, coût %d\n", current->level, current->lower_bound);

        // TODO: Si tous les sommets sont visités, fin
        if (current->path_length == size) {
            printf("Chemin trouvé ! Coût : %d\n", current->lower_bound);
            break;
        }

        int i, j;
        find_best_zero(current->matrix, size, &i, &j);

        // Branche inclusion (i, j)
        double** include_matrix = copy_matrix(current->matrix, size);

        // Mettre la ligne i et colonne j à INF (car on va visiter j après i)
        for (int k = 0; k < size; k++) {
            include_matrix[i][k] = INT_MAX;
            include_matrix[k][j] = INT_MAX;
        }
        // Interdit de revenir (cycle)
        include_matrix[j][0] = INT_MAX;

        int* include_path = (int*)malloc(size * sizeof(int));
        memcpy(include_path, current->path, current->path_length * sizeof(int));
        include_path[current->path_length] = j;

        int include_bound = current->lower_bound + reduce_matrix(include_matrix, size);
        TSPNode* include_node = create_node(include_matrix, size, include_path, current->path_length + 1, include_bound, current->level + 1);
        push(pq, include_node);

        // Branche exclusion (interdire i → j)
        double** exclude_matrix = copy_matrix(current->matrix, size);
        exclude_matrix[i][j] = INT_MAX;

        int exclude_bound = current->lower_bound + reduce_matrix(exclude_matrix, size);
        TSPNode* exclude_node = create_node(exclude_matrix, size, current->path, current->path_length, exclude_bound, current->level + 1);
        push(pq, exclude_node);

        free(include_path);


        free_matrix(current->matrix, size);
        free(current->path);
        free(current);
    }

    free_queue(pq);}

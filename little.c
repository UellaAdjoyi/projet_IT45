#include "little.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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

// Fonction principale (à implémenter ensuite)
void tsp_little(double** distance_matrix, int size) {
    printf("TODO: Implémenter l'algorithme de Little ici.\n");
}

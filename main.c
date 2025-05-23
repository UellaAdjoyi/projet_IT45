#include <stdio.h>
#include "tsp.h"
#include "little.h"

int main() {
//    City* cities = read_tsp_file("data/berlin52.tsp", &num_cities);
//    //double** distance_matrix = compute_distance_matrix(cities, num_cities);

 //   printf("Matrice originale :\n");
  //  print_matrix(distance_matrix, num_cities);

  //  int reduction_cost = reduce_matrix(distance_matrix, num_cities);

   // printf("Matrice réduite :\n");
   // print_matrix(distance_matrix, num_cities);
   // printf("Coût de réduction : %d\n", reduction_cost);
//
   // free_distance_matrix(distance_matrix, num_cities);
   // free_cities(cities);

//TEST AVEC 4 VILLES
    int size = 4;
    double** matrix = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (double*)malloc(size * sizeof(double));
    }

    // Remplissage de la matrice
    double values[4][4] = {
        {INT_MAX, 20, 30, 10},
        {15, INT_MAX, 16, 4},
        {3, 5, INT_MAX, 2},
        {19, 6, 18, INT_MAX}
    };
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = values[i][j];

    printf("Matrice initiale :\n");
    print_matrix(matrix, size);

    int cost = reduce_matrix(matrix, size);

    printf("Matrice réduite : (coût = %d)\n", cost);
    print_matrix(matrix, size);

    int path[1] = {0};  // On commence à la ville 0 (A)
    TSPNode* root = create_node(matrix, size, path, 1, cost, 0);
    printf("Nœud racine créé. Lower bound: %d\n", root->lower_bound);

    free_matrix(matrix, size);
    free_matrix(root->matrix, size);
    free(root->path);
    free(root);

    return 0;
}

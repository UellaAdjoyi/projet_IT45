#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "little.h"
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "test") == 0) {
        printf("[INFO] Mode test activé\n");
        init_test_matrix();
    } else if (argc == 2) {
        printf("[INFO] Lecture du fichier : %s\n", argv[1]);
        read_tsp_file(argv[1]);
        printf("[INFO] %d villes lues\n", num_cities);
        compute_distance_matrix();
        print_distance_matrix();
    } else {
        printf("Usage: %s fichier.tsp | test\n", argv[0]);
        return 1;
    }

    solve_tsp_little(dist, num_cities);

//    int n = 6;
//    double matrix[MAX_CITIES][MAX_CITIES] = {
//        {INFINITY, 1, 7, 3, 14 ,2},
//        {3,INFINITY,6, 9, 1, 24},
//        {6,4,INFINITY,3, 7, 3 },
//        {2, 3, 5,INFINITY,9, 11},
//        {15, 7, 11, 2,INFINITY, 4},
//        {20, 5, 13, 4 ,18,INFINITY}
//    };
//
//    printf("Résolution du TSP pour %d villes :\n", n);
//    solve_tsp_little(matrix, n);

    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tsp.h"

City* read_tsp_file(const char* filename, int* city_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[256];
    // Avancer jusqu'à NODE_COORD_SECTION
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "DIMENSION", 9) == 0) {
            sscanf(line, "DIMENSION : %d", city_count);
        } else if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            break;
        }
    }

    City* cities = malloc(*city_count * sizeof(City));
    if (!cities) {
        perror("Erreur malloc cities");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *city_count; ++i) {
        fscanf(file, "%d %lf %lf", &cities[i].id, &cities[i].x, &cities[i].y);
    }

    fclose(file);
    return cities;
}

double** compute_distance_matrix(City* cities, int city_count) {
    double** matrix = malloc(city_count * sizeof(double*));
    for (int i = 0; i < city_count; i++) {
        matrix[i] = malloc(city_count * sizeof(double));
        for (int j = 0; j < city_count; j++) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            matrix[i][j] = round(sqrt(dx * dx + dy * dy));
        }
    }
    return matrix;
}

void free_distance_matrix(double** matrix, int city_count) {
    for (int i = 0; i < city_count; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void free_cities(City* cities) {
    free(cities);
}
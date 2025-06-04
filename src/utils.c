//
// Created by Uella Adjoyi on 23/05/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"

City cities[MAX_CITIES];
int num_cities;
double dist[MAX_CITIES][MAX_CITIES];

void read_tsp_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) break;
    }

    num_cities = 0;
    printf("Recherche de NODE_COORD_SECTION...\n");
    while (fgets(line, sizeof(line), f)) {
        int id;
        double x, y;
        if (sscanf(line, "%d %lf %lf", &id, &x, &y) == 3) {
            cities[num_cities].id = id;
            cities[num_cities].x = x;
            cities[num_cities].y = y;
            num_cities++;
        } else break;
        printf("NODE_COORD_SECTION trouvé\n");
        printf("Ville %d : (%.1f, %.1f)\n", id, x, y);

    }

    fclose(f);
}

void compute_distance_matrix() {
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            if (i == j) dist[i][j] = INFINITY;
            else {
                double dx = cities[i].x - cities[j].x;
                double dy = cities[i].y - cities[j].y;
                dist[i][j] = round(sqrt(dx * dx + dy * dy));
            }
        }
    }
}

void print_distance_matrix() {
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            if (dist[i][j] == INFINITY) printf("INF\t");
            else printf("%.0f\t", dist[i][j]);
        }
        printf("\n");
    }
}

void init_test_matrix() {
    num_cities = 4;
    double example[4][4] = {
        {INFINITY, 10, 15, 20},
        {10, INFINITY, 35, 25},
        {15, 35, INFINITY, 30},
        {20, 25, 30, INFINITY}
    };

    for (int i = 0; i < num_cities; i++)
        for (int j = 0; j < num_cities; j++)
            dist[i][j] = example[i][j];
}

//
// Created by Uella Adjoyi on 04/06/2025.
//

#ifndef UTILS_H
#define UTILS_H

#define MAX_CITIES 100

typedef struct {
    int id;
    double x, y;
} City;

extern City cities[MAX_CITIES];
extern int num_cities;
extern double dist[MAX_CITIES][MAX_CITIES];

void read_tsp_file(const char *filename);
void compute_distance_matrix();
void print_distance_matrix();
void init_test_matrix();

#endif //UTILS_H

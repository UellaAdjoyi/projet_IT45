//
// Created by Uella Adjoyi on 23/05/2025.
//

#ifndef TSP_H
#define TSP_H
typedef struct {
    int id;
    double x;
    double y;
} City;

City* read_tsp_file(const char* filename, int* city_count);
double** compute_distance_matrix(City* cities, int city_count);
void free_distance_matrix(double** matrix, int city_count);
void free_cities(City* cities);
#endif //TSP_H

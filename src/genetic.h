//
// Created by Uella Adjoyi on 13/06/2025.
//

#ifndef GENETIC_H
#define GENETIC_H
#define MAX_POP 100
#define MAX_GEN 500
#define MUTATION_RATE 5
#define MAX_N 1000

extern int N;

typedef struct {
    int tour[MAX_N];  // séquence des villes
    int cost;         // coût total
} Individus;

extern Individus population[MAX_POP];

void genetic_algorithm();
int evaluer_cout(int* tour);

#endif //GENETIC_H

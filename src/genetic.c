#include "genetic.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N;
Individus population[MAX_POP];

int evaluer_cout(int *tour) {
    int cost = 0;
    for (int i = 0; i < N - 1; i++)
        cost += (int)dist[tour[i]][tour[i+1]];
    cost += (int)dist[tour[N-1]][tour[0]];
    return cost;
}

void init_individual(Individus *ind) {
    for (int i = 0; i < N; i++)
        ind->tour[i] = i;

    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = ind->tour[i];
        ind->tour[i] = ind->tour[j];
        ind->tour[j] = tmp;
    }

    ind->cost = evaluer_cout(ind->tour);
}

void init_population() {
    for (int i = 0; i < MAX_POP; i++)
        init_individual(&population[i]);
}

Individus selection_tournament() {
    int best = rand() % MAX_POP;
    for (int i = 0; i < 2; i++) {
        int challenger = rand() % MAX_POP;
        if (population[challenger].cost < population[best].cost)
            best = challenger;
    }
    return population[best];
}

void order_crossover(Individus p1, Individus p2, Individus *child) {
    int start = rand() % N;
    int end = rand() % N;
    if (start > end) { int t = start; start = end; end = t; }

    int included[MAX_N] = {0};
    for (int i = start; i <= end; i++) {
        child->tour[i] = p1.tour[i];
        included[child->tour[i]] = 1;
    }

    int idx = (end + 1) % N;
    for (int i = 0; i < N; i++) {
        int city = p2.tour[(end + 1 + i) % N];
        if (!included[city]) {
            child->tour[idx] = city;
            idx = (idx + 1) % N;
            included[city] = 1;
        }
    }
}

void mutation(Individus *ind) {
    if ((rand() % 100) < MUTATION_RATE) {
        int i = rand() % N, j = rand() % N;
        int temp = ind->tour[i];
        ind->tour[i] = ind->tour[j];
        ind->tour[j] = temp;
    }
}

void genetic_algorithm() {
    init_population();

    for (int gen = 0; gen < MAX_GEN; gen++) {
        Individus p1 = selection_tournament();
        Individus p2 = selection_tournament();

        Individus child;
        order_crossover(p1, p2, &child);
        mutation(&child);
        child.cost = evaluer_cout(child.tour);

        int worst_idx = 0;
        for (int i = 1; i < MAX_POP; i++) {
            if (population[i].cost > population[worst_idx].cost)
                worst_idx = i;
        }

        if (child.cost < population[worst_idx].cost)
            population[worst_idx] = child;

        if (gen % 50 == 0) {
            int best_idx = 0;
            for (int i = 1; i < MAX_POP; i++) {
                if (population[i].cost < population[best_idx].cost)
                    best_idx = i;
            }
            printf("Gen %d - Meilleur coût : %d\n", gen, population[best_idx].cost);
        }
    }
}

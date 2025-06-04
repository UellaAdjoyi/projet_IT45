#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define MAX_N 100
#define TABU_TENURE 10
#define MAX_ITER 500

int N;
int D[MAX_N][MAX_N];

int best_solution[MAX_N];
int best_cost = INT_MAX;

int tabu_matrix[MAX_N][MAX_N];

// Lecture d’un fichier .tsp (matrice de distance)
void read_tsp(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Fichier introuvable");
        exit(1);
    }
    fscanf(file, "%d", &N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(file, "%d", &D[i][j]);
    fclose(file);
}

// Évalue le coût total d’un tour
int compute_cost(int sol[MAX_N]) {
    int cost = 0;
    for (int i = 0; i < N - 1; i++) {
        cost += D[sol[i]][sol[i + 1]];
    }
    cost += D[sol[N - 1]][sol[0]];
    return cost;
}

// Solution initiale gloutonne
void greedy_solution(int sol[MAX_N]) {
    bool visited[MAX_N] = {false};
    sol[0] = 0;
    visited[0] = true;

    for (int k = 1; k < N; k++) {
        int last = sol[k - 1];
        int next = -1, min_dist = INT_MAX;
        for (int i = 0; i < N; i++) {
            if (!visited[i] && D[last][i] < min_dist) {
                min_dist = D[last][i];
                next = i;
            }
        }
        sol[k] = next;
        visited[next] = true;
    }
}

// Échange 2 villes (2-opt)
void swap(int sol[MAX_N], int i, int j) {
    while (i < j) {
        int temp = sol[i];
        sol[i] = sol[j];
        sol[j] = temp;
        i++; j--;
    }
}

// Algorithme Tabou
void tabu_search() {
    int current[MAX_N], best_local[MAX_N];
    greedy_solution(current);
    memcpy(best_solution, current, sizeof(int) * N);
    int current_cost = compute_cost(current);
    best_cost = current_cost;

    memset(tabu_matrix, 0, sizeof(tabu_matrix));

    for (int iter = 0; iter < MAX_ITER; iter++) {
        int best_neighbor_cost = INT_MAX;
        int move_i = -1, move_j = -1;

        // Génération de voisinage 2-opt
        for (int i = 1; i < N - 1; i++) {
            for (int j = i + 1; j < N; j++) {
                int neighbor[MAX_N];
                memcpy(neighbor, current, sizeof(int) * N);
                swap(neighbor, i, j);
                int delta = compute_cost(neighbor);

                if ((tabu_matrix[current[i]][current[j]] == 0 || delta < best_cost) && delta < best_neighbor_cost) {
                    best_neighbor_cost = delta;
                    move_i = i;
                    move_j = j;
                    memcpy(best_local, neighbor, sizeof(int) * N);
                }
            }
        }

        if (move_i != -1) {
            // Appliquer le meilleur voisin
            memcpy(current, best_local, sizeof(int) * N);
            current_cost = best_neighbor_cost;

            // Mise à jour tabou
            tabu_matrix[current[move_i]][current[move_j]] = TABU_TENURE;
            tabu_matrix[current[move_j]][current[move_i]] = TABU_TENURE;

            // Mise à jour meilleure solution globale
            if (current_cost < best_cost) {
                best_cost = current_cost;
                memcpy(best_solution, current, sizeof(int) * N);
            }
        }

        // Décrémenter tous les tabous
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (tabu_matrix[i][j] > 0)
                    tabu_matrix[i][j]--;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilisation : %s fichier.tsp\n", argv[0]);
        return 1;
    }

    srand(time(NULL));
    read_tsp(argv[1]);

    tabu_search();

    printf("Coût optimal trouvé (tabou) : %d\n", best_cost);
    printf("Chemin : ");
    for (int i = 0; i < N; i++) {
        printf("%d -> ", best_solution[i]);
    }
    printf("%d\n", best_solution[0]);

    return 0;
}

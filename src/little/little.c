#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

#define N_MAX 100

int N; // Nombre de villes
int D[N_MAX][N_MAX]; // Matrice de distances
int best_cost = INT_MAX; // Coût optimal trouvé
int best_path[N_MAX]; // Meilleur chemin
bool visited[N_MAX]; // Marquage des villes visitées

// Lecture de fichier .tsp (format simplifié : coordonnées ou matrice symétrique)
void read_tsp(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Fichier introuvable");
        exit(1);
    }

    fscanf(f, "%d", &N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(f, "%d", &D[i][j]);

    fclose(f);
}

// Réduction de la matrice et calcul du coût minimal
int reduce_matrix(int mat[N_MAX][N_MAX], int n) {
    int cost = 0;

    // Réduction par lignes
    for (int i = 0; i < n; i++) {
        int row_min = INT_MAX;
        for (int j = 0; j < n; j++)
            if (mat[i][j] < row_min && mat[i][j] != INT_MAX)
                row_min = mat[i][j];
        if (row_min != INT_MAX && row_min != 0) {
            cost += row_min;
            for (int j = 0; j < n; j++)
                if (mat[i][j] != INT_MAX)
                    mat[i][j] -= row_min;
        }
    }

    // Réduction par colonnes
    for (int j = 0; j < n; j++) {
        int col_min = INT_MAX;
        for (int i = 0; i < n; i++)
            if (mat[i][j] < col_min && mat[i][j] != INT_MAX)
                col_min = mat[i][j];
        if (col_min != INT_MAX && col_min != 0) {
            cost += col_min;
            for (int i = 0; i < n; i++)
                if (mat[i][j] != INT_MAX)
                    mat[i][j] -= col_min;
        }
    }

    return cost;
}

// Résolution récursive (branch and bound)
void tsp(int level, int current, int cost, int path[N_MAX], bool visited[N_MAX]) {
    if (level == N) {
        int total_cost = cost + D[current][path[0]];
        if (total_cost < best_cost) {
            best_cost = total_cost;
            memcpy(best_path, path, sizeof(int) * N);
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            visited[i] = true;
            path[level] = i;
            int new_cost = cost + D[current][i];
            if (new_cost < best_cost) {
                tsp(level + 1, i, new_cost, path, visited);
            }
            visited[i] = false;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilisation : %s fichier.tsp\n", argv[0]);
        return 1;
    }

    read_tsp(argv[1]);

    int path[N_MAX];
    memset(visited, 0, sizeof(visited));
    path[0] = 0;
    visited[0] = true;

    tsp(1, 0, 0, path, visited);

    printf("Coût optimal trouvé : %d\n", best_cost);
    printf("Chemin : ");
    for (int i = 0; i < N; i++) printf("%d -> ", best_path[i]);
    printf("%d\n", best_path[0]); // Retour à la ville de départ

    return 0;
}

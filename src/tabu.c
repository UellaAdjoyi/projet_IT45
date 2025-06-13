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
int D[MAX_N][MAX_N];  // Matrice de distances

int best_solution[MAX_N];
int best_cost = INT_MAX;

int tabu_matrix[MAX_N][MAX_N];

/// === Lecture de la matrice de distances depuis un fichier .tsp ===
void charger_matrice(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(file, "%d", &D[i][j]);

    fclose(file);
}

/// === Évaluation du coût total d'un cycle (solution) ===
int evaluer_cout(int sol[MAX_N]) {
    int cout = 0;
    for (int i = 0; i < N - 1; i++)
        cout += D[sol[i]][sol[i + 1]];
    cout += D[sol[N - 1]][sol[0]];  // Retour au départ
    return cout;
}

/// === Construction d'une solution initiale gloutonne ===
void solution_gloutonne(int sol[MAX_N]) {
    bool visite[MAX_N] = {false};
    sol[0] = 0;
    visite[0] = true;

    for (int k = 1; k < N; k++) {
        int dernier = sol[k - 1];
        int suivant = -1;
        int dist_min = INT_MAX;

        for (int i = 0; i < N; i++) {
            if (!visite[i] && D[dernier][i] < dist_min) {
                dist_min = D[dernier][i];
                suivant = i;
            }
        }
        sol[k] = suivant;
        visite[suivant] = true;
    }
}

/// === Inverser une sous-séquence de villes (2-opt) ===
void inversion_segment(int sol[MAX_N], int i, int j) {
    while (i < j) {
        int tmp = sol[i];
        sol[i] = sol[j];
        sol[j] = tmp;
        i++; j--;
    }
}

/// === Algorithme de recherche tabou ===
void recherche_tabou() {
    int courant[MAX_N], voisin_opt[MAX_N];
    solution_gloutonne(courant);
    memcpy(best_solution, courant, sizeof(int) * N);
    best_cost = evaluer_cout(courant);

    memset(tabu_matrix, 0, sizeof(tabu_matrix));

    for (int iter = 0; iter < MAX_ITER; iter++) {
        int meilleur_voisin_cout = INT_MAX;
        int move_i = -1, move_j = -1;

        for (int i = 1; i < N - 1; i++) {
            for (int j = i + 1; j < N; j++) {
                int voisin[MAX_N];
                memcpy(voisin, courant, sizeof(int) * N);
                inversion_segment(voisin, i, j);

                int cout_voisin = evaluer_cout(voisin);

                bool deplacement_tabou = tabu_matrix[courant[i]][courant[j]] > 0;
                bool critere_aspiration = (cout_voisin < best_cost);

                if ((!deplacement_tabou || critere_aspiration) && cout_voisin < meilleur_voisin_cout) {
                    meilleur_voisin_cout = cout_voisin;
                    move_i = i;
                    move_j = j;
                    memcpy(voisin_opt, voisin, sizeof(int) * N);
                }
            }
        }

        if (move_i != -1) {
            memcpy(courant, voisin_opt, sizeof(int) * N);

            tabu_matrix[courant[move_i]][courant[move_j]] = TABU_TENURE;
            tabu_matrix[courant[move_j]][courant[move_i]] = TABU_TENURE;

            if (meilleur_voisin_cout < best_cost) {
                best_cost = meilleur_voisin_cout;
                memcpy(best_solution, courant, sizeof(int) * N);
            }
        }

        // Décrément du temps tabou
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (tabu_matrix[i][j] > 0)
                    tabu_matrix[i][j]--;
    }
}

/// === Affichage de la meilleure solution trouvée ===
void afficher_resultat() {
    printf("Coût optimal trouvé (Tabou) : %d\n", best_cost);
    printf("Chemin : ");
    for (int i = 0; i < N; i++)
        printf("%d -> ", best_solution[i]);
    printf("%d\n", best_solution[0]);
}

/// === Point d'entrée du programme ===
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Utilisation : %s fichier.tsp\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    charger_matrice(argv[1]);
    recherche_tabou();
    afficher_resultat();

    return EXIT_SUCCESS;
}

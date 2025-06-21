#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "little.h"
#include "tabu.h"
#include "genetic.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>



int main(int argc, char *argv[]) {
  //TEST LITTLE
   /*if (argc == 2 && strcmp(argv[1], "test") == 0) {
        printf("🔧 Mode test interne (matrice définie en dur)\n");
        init_test_matrix();
    } else if (argc == 2) {
        printf("📥 Lecture du fichier : %s\n", argv[1]);
        read_tsp_file(argv[1]);
        printf("✅ %d villes lues\n", num_cities);
        compute_distance_matrix();
    } else {
        printf("❗️ Utilisation : %s fichier.tsp | test\n", argv[0]);
        return EXIT_FAILURE;
    }

    clock_t start = clock();

    solve_tsp_little(dist, num_cities);
	printf("\n📌 Chemin optimal : ");
	for (int i = 0; i < num_cities; i++) {
    printf("%d → ", best_path[i]);
	}
	printf("%d\n", best_path[0]); // retour à la ville de départ

	printf("✅ Coût total : %.2f\n", best_cost);

    clock_t end = clock();
    double temps = (double)(end - start) / CLOCKS_PER_SEC;
    printf("⏱ Temps d'exécution : %.4f secondes\n", temps);*/

//TEST AVEC 6 VILLES
    /*int n = 6;
    double matrix[MAX_CITIES][MAX_CITIES] = {
        {INFINITY, 1, 7, 3, 14 ,2},
        {3,INFINITY,6, 9, 1, 24},
        {6,4,INFINITY,3, 7, 3 },
        {2, 3, 5,INFINITY,9, 11},
        {15, 7, 11, 2,INFINITY, 4},
        {20, 5, 13, 4 ,18,INFINITY}
    };*/

    //printf("Résolution du TSP pour %d villes :\n", n);
    //solve_tsp_little(matrix, n);

    //TEST TABU
      if (argc < 2) {
        fprintf(stderr, "Utilisation : %s fichier.tsp\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    charger_matrice(argv[1]);

    clock_t start = clock();  // Début du chrono

    recherche_tabou();

    clock_t end = clock();    // Fin du chrono
    double temps = (double)(end - start) / CLOCKS_PER_SEC;

    afficher_resultat();

    printf("⏱ Temps d'exécution : %.4f secondes\n", temps);

    // Ajoute ici l’écart à l’optimal
    double optimal_connu = 7542.0;  // Pour berlin52.tsp
    if (optimal_connu > 0) {
        double ecart = 100.0 * (best_cost - optimal_connu) / optimal_connu;
        printf("📊 Écart relatif à l'optimal : %.2f %%\n", ecart);
    }
    //TEST GENETIC
     /*if (argc < 2) {
        printf("Usage: %s fichier.tsp\n", argv[0]);
        return 1;
    }

    read_tsp_file(argv[1]);
    compute_distance_matrix();
    N = num_cities;
    printf("Nombre de villes : %d\n", N);

    srand((unsigned int)time(NULL));
    clock_t start = clock();

    genetic_algorithm();

    clock_t end = clock();
    double temps = (double)(end - start) / CLOCKS_PER_SEC;

    int best_idx = 0;
    for (int i = 1; i < MAX_POP; i++) {
        if (population[i].cost < population[best_idx].cost)
            best_idx = i;
    }

    printf("\n✅ Meilleur coût trouvé (AG) : %d\n", population[best_idx].cost);
    printf("🧭 Tour : ");
    for (int i = 0; i < N; i++)
        printf("%d ", population[best_idx].tour[i]);
    printf("%d\n", population[best_idx].tour[0]);

    printf("⏱ Temps d'exécution : %.4f secondes\n", temps);

    double optimal_connu = 7542.0; // change selon l'instance
    if (optimal_connu > 0) {
        double ecart = 100.0 * (population[best_idx].cost - optimal_connu) / optimal_connu;
        printf("📊 Écart par rapport à l’optimal : %.2f %%\n", ecart);
    }*/



    return 0;

}

// little.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "little.h"
#include "utils.h"

static double best_cost = INFINITY;
static int best_path[MAX_CITIES];

void copy_matrix(double dest[MAX_CITIES][MAX_CITIES], double src[MAX_CITIES][MAX_CITIES], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            dest[i][j] = src[i][j];
}

double reduce_matrix(double matrix[MAX_CITIES][MAX_CITIES], int size) {
    double reduction_cost = 0;

    // Réduction des lignes
    for (int i = 0; i < size; i++) {
        double min = INFINITY;
        for (int j = 0; j < size; j++)
            if (matrix[i][j] < min)
                min = matrix[i][j];
        if (min != INFINITY && min > 0) {
            reduction_cost += min;
            for (int j = 0; j < size; j++)
                if (matrix[i][j] != INFINITY)
                    matrix[i][j] -= min;
        }
    }

    // Réduction des colonnes
    for (int j = 0; j < size; j++) {
        double min = INFINITY;
        for (int i = 0; i < size; i++)
            if (matrix[i][j] < min)
                min = matrix[i][j];
        if (min != INFINITY && min > 0) {
            reduction_cost += min;
            for (int i = 0; i < size; i++)
                if (matrix[i][j] != INFINITY)
                    matrix[i][j] -= min;
        }
    }

    return reduction_cost;
}

ZeroPenalty find_best_zero(double matrix[MAX_CITIES][MAX_CITIES], int size) {
    ZeroPenalty best = {-1, -1, -1.0};

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == 0.0) {
                double row_min = INFINITY;
                double col_min = INFINITY;

                for (int k = 0; k < size; k++) {
                    if (k != j && matrix[i][k] < row_min)
                        row_min = matrix[i][k];
                    if (k != i && matrix[k][j] < col_min)
                        col_min = matrix[k][j];
                }

                double penalty = (row_min == INFINITY ? 0 : row_min) +
                                 (col_min == INFINITY ? 0 : col_min);

                if (penalty > best.penalty) {
                    best.row = i;
                    best.col = j;
                    best.penalty = penalty;
                }
            }
        }
    }

    return best;
}

int forms_subtour(int path[], int len, int from, int to) {
    int count = 1, city = to;

    while (count < len) {
        int found = 0;
        for (int i = 0; i < len; i++) {
            if (path[i] == city) {
                city = path[(i + 1) % len];
                found = 1;
                break;
            }
        }
        if (!found) break;
        if (city == from) return 1;
        count++;
    }
    return 0;
}


void branch(Node parent, Edge e, Node *include, Node *exclude) {
    // Inclusion
    copy_matrix(include->matrix, parent.matrix, parent.size);
    include->size = parent.size;
    include->lower_bound = parent.lower_bound;
    memcpy(include->path, parent.path, sizeof(int) * parent.path_len);
    include->path[include->path_len = parent.path_len] = e.from;
    include->path_len++;

    for (int i = 0; i < include->size; i++) {
        include->matrix[e.from][i] = INFINITY;
        include->matrix[i][e.to] = INFINITY;
    }
    include->matrix[e.to][e.from] = INFINITY;

    include->lower_bound += reduce_matrix(include->matrix, include->size);

    // Exclusion
    copy_matrix(exclude->matrix, parent.matrix, parent.size);
    exclude->size = parent.size;
    exclude->lower_bound = parent.lower_bound;
    memcpy(exclude->path, parent.path, sizeof(int) * parent.path_len);
    exclude->path_len = parent.path_len;
    exclude->matrix[e.from][e.to] = INFINITY;

    exclude->lower_bound += reduce_matrix(exclude->matrix, exclude->size);

    if (forms_subtour(parent.path, parent.path_len, e.from, e.to)) {
        // Ne pas inclure ce bord car il forme un sous-tour
        include->lower_bound = INFINITY;
        return;
    }

}

void solve_little(Node current) {
    printf("solve_little: len=%d, bound=%.2f\n", current.path_len, current.lower_bound);

    if (current.path_len == num_cities) {
        int last = current.path[current.path_len - 1];
        int first = current.path[0];
        double total_cost = current.lower_bound + current.matrix[last][first];

        if (total_cost < best_cost) {
            best_cost = total_cost;
            memcpy(best_path, current.path, sizeof(int) * num_cities);
        }
        return;
    }

    ZeroPenalty zp = find_best_zero(current.matrix, current.size);
    if (zp.row == -1 || zp.col == -1) return;

    Edge e = {zp.row, zp.col};

    Node left, right;
    branch(current, e, &left, &right);

    if (left.lower_bound < best_cost)
        solve_little(left);
    if (right.lower_bound < best_cost)
        solve_little(right);
}


void solve_tsp_little(double original_matrix[MAX_CITIES][MAX_CITIES], int n) {
    printf("Appel de solve_tsp_little avec %d villes\n", n);

    num_cities = n;

    Node root;
    root.size = n;
    copy_matrix(root.matrix, original_matrix, n);
    root.lower_bound = reduce_matrix(root.matrix, n);
    root.path_len = 0;

    best_cost = INFINITY;

    solve_little(root);

    printf("Meilleur coût trouvé : %.2f\n", best_cost);
    printf("Chemin : ");
    for (int i = 0; i < num_cities; i++) {
        printf("%d → ", best_path[i]);
    }
    printf("%d\n", best_path[0]); // Retour à la ville initiale
}

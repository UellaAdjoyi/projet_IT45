#ifndef LITTLE_H
#define LITTLE_H

#define MAX_CITIES 100

typedef struct {
    double matrix[MAX_CITIES][MAX_CITIES];
    int size;
    double lower_bound;
    int path[MAX_CITIES];
    int path_len;
} Node;
extern double best_cost;
extern int best_path[MAX_CITIES];

typedef struct {
    int from;
    int to;
} Edge;

typedef struct {
    int row;
    int col;
    double penalty;
} ZeroPenalty;

void copy_matrix(double dest[MAX_CITIES][MAX_CITIES], double src[MAX_CITIES][MAX_CITIES], int size);
double reduce_matrix(double matrix[MAX_CITIES][MAX_CITIES], int size);
ZeroPenalty find_best_zero(double matrix[MAX_CITIES][MAX_CITIES], int size);
void branch(Node parent, Edge e, Node *include, Node *exclude);
void solve_tsp_little(double original_matrix[MAX_CITIES][MAX_CITIES], int n);

#endif

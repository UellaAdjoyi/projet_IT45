#ifndef LITTLE_H
#define LITTLE_H
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct {
   double** matrix;   //Matrice des coûts réduits
   int size;       //Nombre de villes restantes
   int* path;// Chemin partel
   int path_length;//Longueur du chemin
   int lower_bound;//Borne inférieure
   int level; //Niveau dans l'arbre
} TSPNode;

//Les différentes fonctions
TSPNode* create_node(double** matrix, int size, int* path, int path_length, int lower_bound, int level);
int reduce_matrix(double** matrix, int size);
void print_matrix(double** matrix, int size);
double** copy_matrix(double** matrix, int size);
void free_matrix(double** matrix, int size);
void free_node(TSPNode* node);
void tsp_little(double** distance_matrix, int size);

typedef struct {
   TSPNode** nodes;
   int size;
   int capacity;
} PriorityQueue;

PriorityQueue* create_queue(int capacity);
void push(PriorityQueue* pq, TSPNode* node);
TSPNode* pop(PriorityQueue* pq);
void free_queue(PriorityQueue* pq);


#endif //LITTLE_H

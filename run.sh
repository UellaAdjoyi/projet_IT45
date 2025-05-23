#!/bin/bash

# Compiler le projet
echo "Compilation..."
make

# Exécuter avec un fichier exemple
echo "Exécution sur berlin52.tsp (10 premières villes)..."
./build/tsp_solver data/berlin52.tsp

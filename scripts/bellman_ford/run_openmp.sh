#!/bin/bash

# Fichier de sortie
output_file="res_cpu.data"
echo "# Nb_Vertices Nb_Edges Temps_Total" > $output_file
# Boucles sur les différentes tailles
for vertices in 100 500 1000 5000 10000 15000; do
    edges=$((vertices - 1))
    edges=$((edges * $vertices))
    edges=$((edges / 2))
    echo "$vertices"
    
    # Mise à jour de la définition de MATRIX_SIZE dans le fichier source
    sed -i "s/#define VERTICES.*/#define VERTICES $vertices/" bellman_cpu.c
    sed -i "s/#define EDGES.*/#define EDGES $edges/" bellman_cpu.c
    
    # Compilation
    gcc -O2 -o bellman_cpu bellman_cpu.c -fopenmp -lm
    
    # Exécution et récupération des résultats
    result=$(./bellman_cpu)
    
    # Extraction des informations pertinentes
    total_time=$(echo "$result" | awk '{print $5}')
    
    # Écriture dans le fichier
    echo "$vertices $edges $total_time" >> $output_file
done

echo "Résultats enregistrés dans $output_file"


#!/bin/bash

# Fichier de sortie
output_file="res_cuda.data"
echo "# Nb_Vertices Nb_Edges Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Boucles sur les différentes tailles
for vertices in 100 500 1000 5000 10000 15000; do
    edges=$((vertices - 1))
    edges=$((edges * $vertices))
    edges=$((edges / 2))
    echo "$vertices"
    
    # Mise à jour de la définition de MATRIX_SIZE dans le fichier source
    sed -i "s/#define VERTICES.*/#define VERTICES $vertices/" bellman_cuda.cu
    sed -i "s/#define EDGES.*/#define EDGES $edges/" bellman_cuda.cu
    
    # Compilation
    nvcc -o bellman_cuda bellman_cuda.cu -lm
    
    # Exécution et récupération des résultats
    result=$(./bellman_cuda)
    
    # Extraction des informations pertinentes
    total_time=$(echo "$result" | awk '{print $4}')
    gpu_time=$(echo "$result" | awk '{print $9}')
    transfer_time=$(echo "$result" | awk '{print $14}')
    
    # Écriture dans le fichier
    echo "$vertices $edges $total_time $gpu_time $transfer_time" >> $output_file
done

echo "Résultats enregistrés dans $output_file"


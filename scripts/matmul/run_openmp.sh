#!/bin/bash

# Fichier de sortie
output_file="res_openmp.data"
echo "# Taille Temps_Total" > $output_file

# Boucles sur les différentes tailles
for size in 256 512 1024 2048 4096 8192 16384; do
    # Mise à jour de la définition de MATRIX_SIZE dans le fichier source
    sed -i "s/#define MATRIX_SIZE.*/#define MATRIX_SIZE $size/" matmul_openmp.c
    
    # Compilation
    gcc -O2 -o matmul_openmp matmul_openmp.c -fopenmp -lm
    
    # Exécution et récupération des résultats
    result=$(./matmul_openmp)
    
    # Extraction des informations pertinentes
    total_time=$(echo "$result" | awk '{print $7}')
    
    # Écriture dans le fichier
    echo "$size $total_time" >> $output_file
done

echo "Résultats enregistrés dans $output_file"


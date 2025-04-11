#!/bin/bash

# Fichier de sortie
output_file="res_opencl.data"
echo "# Taille Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Boucles sur les différentes tailles
for size in 256 512 1024 2048 4096 8192 16384; do
    # Mise à jour de la définition de MATRIX_SIZE dans le fichier source
    sed -i "s/#define MATRIX_SIZE.*/#define MATRIX_SIZE $size/" matmul_opencl.c
    
    # Compilation
    gcc -o matmul_opencl matmul_opencl.c -lOpenCL -lm
    
    # Exécution et récupération des résultats
    result=$(./matmul_opencl)
    
    # Extraction des informations pertinentes
    total_time=$(echo "$result" | awk '{print $7}')
    gpu_time=$(echo "$result" | awk '{print $12}')
    transfer_time=$(echo "$result" | awk '{print $17}')
    
    # Écriture dans le fichier
    echo "$size $total_time $gpu_time $transfer_time" >> $output_file
done

echo "Résultats enregistrés dans $output_file"

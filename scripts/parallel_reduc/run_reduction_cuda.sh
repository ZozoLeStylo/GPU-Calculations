#!/bin/bash

# Fichier de sortie
output_file="res_reduction_cuda.data"
echo "# Taille Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Boucles sur les différentes tailles
for size in 256 512 1024 2048 4096 8192 16384; do
    # Mise à jour de la définition de DATA_SIZE dans le fichier source
    sed -i "s/#define DATA_SIZE.*/#define DATA_SIZE $size/" reduction_cuda.c
    
    # Compilation
    nvcc -o reduction_cuda reduction_cuda.cu -lm
    
    # Exécution et récupération des résultats
    result=$(./reduction_cuda)
    
    # Extraction des informations pertinentes
    total_time=$(echo "$result" | awk '{print $7}')
    gpu_time=$(echo "$result" | awk '{print $12}')
    transfer_time=$(echo "$result" | awk '{print $17}')
    
    # Écriture dans le fichier
    echo "$size $total_time $gpu_time $transfer_time" >> $output_file
done

echo "Résultats enregistrés dans $output_file"


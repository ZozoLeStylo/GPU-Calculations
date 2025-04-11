#!/bin/bash

# Output file
output_file="res_heat_openmp.data"
echo "# Taille Temps_Total" > $output_file

# Loop over grid sizes
for size in 256 512 1024 2048 4096 8192 16384 32768; do
    sed -i "s/#define GRID_SIZE.*/#define GRID_SIZE $size/" heat_diffusion_openmp.c
    gcc -O2 -o heat_openmp heat_diffusion_openmp.c -fopenmp -lm
    result=$(./heat_openmp)
    total_time=$(echo "$result" | awk '{print $10}')
    echo "$size $total_time" >> $output_file
done
echo "Résultats enregistrés dans $output_file"


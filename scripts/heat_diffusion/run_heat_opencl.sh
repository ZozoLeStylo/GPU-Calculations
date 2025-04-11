#!/bin/bash

# Output file
output_file="res_heat_opencl.data"
echo "# Taille Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Loop over grid sizes
for size in 256 512 1024 2048 4096 8192 16384 32768; do
    sed -i "s/#define GRID_SIZE.*/#define GRID_SIZE $size/" heat_diffusion_opencl.c
    gcc -o heat_opencl heat_diffusion_opencl.c -lOpenCL -lm
    result=$(./heat_opencl)
    
    # Extract timing information
    total_time=$(echo "$result" | awk '{print $10}')
    gpu_time=$(echo "$result" | awk '{print $15}')
    transfer_time=$(echo "$result" | awk '{print $20}')

    # Append to file
    echo "$size $total_time $gpu_time $transfer_time" >> $output_file
done
echo "Résultats enregistrés dans $output_file"


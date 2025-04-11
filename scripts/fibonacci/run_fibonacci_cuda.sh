#!/bin/bash

# Output file
output_file="res_fibonacci_cuda.data"
echo "# N Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Values of N to test
for n in 1 3 5 7 9 11 13 15; do
    sed -i "s/int n = .*/int n = $n;/" fibonacci_cuda.cu
    nvcc -o fibonacci_cuda fibonacci_cuda.cu -lm
    result=$(./fibonacci_cuda)

    # Extract timing information
    total_time=$(echo "$result" | awk '{print $7}')
    gpu_time=$(echo "$result" | awk '{print $12}')
    transfer_time=$(echo "$result" | awk '{print $17}')

    # Append to file
    echo "$size $total_time $gpu_time $transfer_time" >> $output_file
done
echo "Résultats enregistrés dans $output_file"


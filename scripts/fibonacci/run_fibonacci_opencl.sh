#!/bin/bash

# Output file
output_file="res_fibonacci_opencl.data"
echo "# N Temps_Total Temps_GPU Temps_Transfert" > $output_file

# Values of N to test
for n in 1 3 5 7 9 11 13 15; do
    sed -i "s/int n = .*/int n = $n;/" fibonacci_opencl.c
    gcc -o fibonacci_opencl fibonacci_opencl.c -lOpenCL -lm
    result=$(./fibonacci_opencl)

    # Extract timing information
    total_time=$(echo "$result" | awk '{print $12}')
    gpu_time=$(echo "$result" | awk '{print $17}')
    transfer_time=$(echo "$result" | awk '{print $22}')

    # Append to file
    echo "$n $total_time $gpu_time $transfer_time" >> $output_file
done
echo "Résultats enregistrés dans $output_file"


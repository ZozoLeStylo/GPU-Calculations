#!/bin/bash

# Output file
output_file="res_fibonacci_openmp.data"
echo "# N Temps_Total" > $output_file

# Values of N to test
for n in 1 3 5 7 9 11 13 15; do
    sed -i "s/int n = .*/int n = $n;/" fibonacci_openmp.c
    gcc -O2 -o fibonacci_openmp fibonacci_openmp.c -lm
    result=$(./fibonacci_openmp)

    # Extract timing information
    total_time=$(echo "$result" | awk '{print $12}')

    # Append to file
    echo "$n $total_time" >> $output_file
done
echo "Résultats enregistrés dans $output_file"


# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Valeur de N (Taille du problème)"
set ylabel "Temps (s)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "graph_fibonacci_cpu_vs_amd.png"
set title "Calcul Fibonacci : CPU vs GPU AMD (OpenCL)"
plot "res_fibonacci_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_fibonacci_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 2 : GPU Nvidia vs GPU AMD
set output "graph_fibonacci_gpu_nvidia_vs_amd.png"
set title "Calcul Fibonacci : GPU Nvidia vs GPU AMD"
plot "res_fibonacci_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_fibonacci_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 3 : Temps totaux pour CPU, GPU Nvidia et GPU AMD
set output "graph_fibonacci_total_comparison.png"
set title "Calcul Fibonacci : Temps totaux CPU, GPU Nvidia et GPU AMD"
plot "res_fibonacci_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_fibonacci_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_fibonacci_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 4 : Tous les résultats pour GPU
set output "graph_fibonacci_gpu_all_results.png"
set title "Calcul Fibonacci : Détails GPU (CUDA et OpenCL)"
plot "res_fibonacci_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_fibonacci_cuda.data" using 1:3 title "GPU Nvidia (CUDA) - Temps GPU" with linespoints, \
     "res_fibonacci_cuda.data" using 1:4 title "GPU Nvidia (CUDA) - Temps transfert" with linespoints, \
     "res_fibonacci_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints, \
     "res_fibonacci_opencl.data" using 1:3 title "GPU AMD (OpenCL) - Temps GPU" with linespoints, \
     "res_fibonacci_opencl.data" using 1:4 title "GPU AMD (OpenCL) - Temps transfert" with linespoints


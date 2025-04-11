# Fichier de sortie : PNG
set terminal png size 800,600

# Abscisse et ordonnée
set xlabel "Taille des données (N)"
set ylabel "Temps (s)"
set grid

# Graphique 1 : CPU et GPU AMD (OpenCL)
set output "graph_cpu_vs_amd.png"
set title "Comparaison CPU vs GPU AMD (OpenCL)"
plot "res_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints \
     "res_opencl.data" using 1:4 title "GPU AMD (OpenCL) - Temps transfert" with linespoints

# Graphique 2 : GPU Nvidia vs GPU AMD
set output "graph_gpu_nvidia_vs_amd.png"
set title "Comparaison GPU Nvidia vs GPU AMD"
plot "res_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 3 : Temps totaux pour CPU, GPU Nvidia, GPU AMD
set output "graph_total_comparison.png"
set title "Temps totaux pour CPU, GPU Nvidia et GPU AMD"
plot "res_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 4 : Tous les résultats
set output "graph_all_results.png"
set title "Tous les résultats"
plot "res_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_cuda.data" using 1:3 title "GPU Nvidia (CUDA) - Temps GPU" with linespoints, \
     "res_cuda.data" using 1:4 title "GPU Nvidia (CUDA) - Temps transfert" with linespoints, \
     "res_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints, \
     "res_opencl.data" using 1:3 title "GPU AMD (OpenCL) - Temps GPU" with linespoints, \
     "res_opencl.data" using 1:4 title "GPU AMD (OpenCL) - Temps transfert" with linespoints


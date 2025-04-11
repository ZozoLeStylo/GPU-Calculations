# Fichier de sortie : PNG
set terminal png size 800,600

# Abscisse et ordonnée
set xlabel "Taille des données (N)"
set ylabel "Temps (s)"
set grid

# Graphique 1 : CPU et GPU AMD (OpenCL)
set output "graph_reduction_cpu_vs_amd.png"
set title "Réduction parallèle : CPU vs GPU AMD (OpenCL)"
plot "res_reduction_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_reduction_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 2 : GPU Nvidia vs GPU AMD
set output "graph_reduction_gpu_nvidia_vs_amd.png"
set title "Réduction parallèle : GPU Nvidia vs GPU AMD"
plot "res_reduction_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_reduction_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 3 : Temps totaux pour CPU, GPU Nvidia, GPU AMD
set output "graph_reduction_total_comparison.png"
set title "Réduction parallèle : Temps totaux CPU, GPU Nvidia, GPU AMD"
plot "res_reduction_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_reduction_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_reduction_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 4 : Tous les résultats
set output "graph_reduction_all_results.png"
set title "Réduction parallèle : Tous les résultats"
plot "res_reduction_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_reduction_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_reduction_cuda.data" using 1:3 title "GPU Nvidia (CUDA) - Temps GPU" with linespoints, \
     "res_reduction_cuda.data" using 1:4 title "GPU Nvidia (CUDA) - Temps transfert" with linespoints, \
     "res_reduction_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints, \
     "res_reduction_opencl.data" using 1:3 title "GPU AMD (OpenCL) - Temps GPU" with linespoints, \
     "res_reduction_opencl.data" using 1:4 title "GPU AMD (OpenCL) - Temps transfert" with linespoints


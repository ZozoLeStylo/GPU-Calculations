# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Taille de la grille (N)"
set ylabel "Temps (s)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "graph_heat_cpu_vs_amd.png"
set title "Diffusion thermique : CPU vs GPU AMD (OpenCL)"
plot "res_heat_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_heat_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 2 : GPU Nvidia vs GPU AMD
set output "graph_heat_gpu_nvidia_vs_amd.png"
set title "Diffusion thermique : GPU Nvidia vs GPU AMD"
plot "res_heat_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_heat_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 3 : Temps totaux pour CPU, GPU Nvidia, GPU AMD
set output "graph_heat_total_comparison.png"
set title "Diffusion thermique : Temps totaux CPU, GPU Nvidia et GPU AMD"
plot "res_heat_openmp.data" using 1:2 title "CPU (OpenMP)" with linespoints, \
     "res_heat_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_heat_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints

# Graphique 4 : Tous les résultats pour GPU
set output "graph_heat_gpu_all_results.png"
set title "Diffusion thermique : Détails GPU (CUDA et OpenCL)"
plot "res_heat_cuda.data" using 1:2 title "GPU Nvidia (CUDA) - Temps total" with linespoints, \
     "res_heat_cuda.data" using 1:3 title "GPU Nvidia (CUDA) - Temps GPU" with linespoints, \
     "res_heat_cuda.data" using 1:4 title "GPU Nvidia (CUDA) - Temps transfert" with linespoints, \
     "res_heat_opencl.data" using 1:2 title "GPU AMD (OpenCL) - Temps total" with linespoints, \
     "res_heat_opencl.data" using 1:3 title "GPU AMD (OpenCL) - Temps GPU" with linespoints, \
     "res_heat_opencl.data" using 1:4 title "GPU AMD (OpenCL) - Temps transfert" with linespoints


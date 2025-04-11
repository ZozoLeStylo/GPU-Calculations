# Fichier de sortie : PNG
set terminal png size 800,600

# Abscisse et ordonnée
set xlabel "Size of matrices"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : GPU AMD (OpenCL)
set output "amdvscpu.png"
set title "GPU AMD (OpenCL)"
plot "res_opencl.data" using 1:2 title "GPU AMD" with linespoints, \
     "res_openmp.data" using 1:2 title "CPU" with linespoints, \
     "res_opencl_gui.data" using 1:2 title "GPU AMD (with GUI)" with linespoints, \
     "res_openmp_gui.data" using 1:2 title "CPU (with GUI)" with linespoints

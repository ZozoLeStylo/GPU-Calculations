# Fichier de sortie : PNG
set terminal png size 800,600

# Abscisse et ordonnée
set xlabel "Size of matrices"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : GPU AMD (OpenCL)
set output "amd.png"
set title "GPU AMD (OpenCL)"
plot "res_opencl.data" using 1:3 title "GPU AMD - GPU Time" with linespoints, \
     "res_opencl.data" using 1:4 title "GPU AMD - Transfer Time" with linespoints


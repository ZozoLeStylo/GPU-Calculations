# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Grid Size"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "amdvscpu.png"
set title "Heat Diffusion : CPU vs GPU AMD (OpenCL)"
plot "res_heat1000_openmp.data" using 1:2 title "CPU" with linespoints, \
     "res_heat1000_opencl.data" using 1:2 title "GPU AMD" with linespoints, \
     "res_heat1000_openmp_gui.data" using 1:2 title "CPU (with GUI)" with linespoints, \
     "res_heat1000_opencl_gui.data" using 1:2 title "GPU AMD (with GUI)" with linespoints

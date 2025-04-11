# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Number n of vertices [(n(n-1))/2 edges]"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "amdvscpu.png"
set title "Bellman-Ford : CPU vs GPU AMD (OpenCL)"
plot "res_cpu.data" using 1:3 title "CPU" with linespoints, \
     "res_opencl.data" using 1:3 title "GPU AMD" with linespoints, \
     "res_cpu_gui.data" using 1:3 title "CPU (with GUI)" with linespoints, \
     "res_opencl_gui.data" using 1:3 title "GPU AMD (with GUI)" with linespoints

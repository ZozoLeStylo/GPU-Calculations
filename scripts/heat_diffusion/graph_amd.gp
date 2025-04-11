# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Number n of vertices [(n(n-1))/2 edges]"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "amd.png"
set title "Heat Diffusion : GPU AMD (OpenCL)"
plot "res_heat1000_opencl.data" using 1:3 title "GPU AMD - GPU Time" with linespoints, \
     "res_heat1000_opencl.data" using 1:4 title "GPU AMD - Transfer Time" with linespoints

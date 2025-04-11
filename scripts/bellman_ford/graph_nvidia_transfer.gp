# Output to PNG
set terminal png size 800,600

# Labels
set xlabel "Number n of vertices [(n(n-1))/2 edges]"
set ylabel "Time (seconds)"
set grid

# Graphique 1 : CPU vs GPU AMD (OpenCL)
set output "cuda.png"
set title "Bellman-Ford : GPU Nvidia (CUDA)"
plot "res_cuda_low.data" using 1:3 title "GPU Nvidia - Total Time" with linespoints, \
     "res_cuda_low.data" using 1:5 title "GPU Nvidia - Transfer Time" with linespoints

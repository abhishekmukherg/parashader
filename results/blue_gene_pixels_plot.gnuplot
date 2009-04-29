set xlabel "Number of pixels"
set ylabel "Time (s)"
set xrange [1000000:4500000]
set yrange [100:240]

set grid ytics

set term png
set output "blue_gene_pixels_plot.png"
pl "blue_gene_pixels_plot.txt" u 1:2 lw 1 title "256 processors" w linespoints

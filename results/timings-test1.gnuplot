set term postscript landscape enhanced mono "DejaVu" 12

set xlabel "Processors"
set ylabel "Time (s)"
set xrange [48:144]
set yrange [100:150]

set grid ytics

set output "timings-test1.eps"
pl "timings-test1.txt" u 1:2 lw 1 title "30 bounces" w linespoints

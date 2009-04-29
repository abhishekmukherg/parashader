set term postscript landscape enhanced mono "DejaVu" 12

set xlabel "Processors"
set ylabel "Time (s)"
set xrange [0:144]
set yrange [0:180]

set grid ytics

set output "timings-test2.png"
pl "timings-test2.txt" u 1:2 lw 1 title "5 bounces" w linespoints

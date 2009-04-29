set xlabel "Bounces"
set ylabel "Time (s)"
set xrange [0:6]
set yrange [4:8]

set grid ytics

set term png
set output "taro_desktop_plot.png"
pl "taro_desktop_plot.txt" u 1:2 lw 1 title "1 processor" w linespoints, "taro_desktop_plot.txt" u 1:3 lw 1 title "2 processors" w linespoints

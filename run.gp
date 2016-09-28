
set xlabel 'N'
set ylabel 'Time(sec)'
set style fill solid
set title 'Compute_pi Time'
set term png enhanced font 'Verdana,10'
set output 'time.png'
set term pngcairo size 1280,960

plot "result_clock_gettime.csv" using 1:2 title 'baseline' with lines lt rgb 'red' lw 3, \
     "result_clock_gettime.csv"	using 1:4 title 'openmp_2' with lines lt rgb 'blue' lw 3, \
     "result_clock_gettime.csv" using 1:6 title 'openmp_4' with lines lt rgb 'green' lw 3

set xlabel 'N'
set ylabel 'Error'
set logscale y
set style fill solid
set title 'Compute_pi Error'
set term png enhanced font 'Verdana,10'
set output 'error.png'
set term pngcairo size 1280,960
set xrange [0:250000]

plot "result_clock_gettime.csv" using 1:3 title 'baseline' with lines, \
     "result_clock_gettime.csv" using 1:5 title 'openmp_2' with lines, \
     "result_clock_gettime.csv" using 1:7 title 'openmp_4' with lines lt rgb 'blue' lw 3


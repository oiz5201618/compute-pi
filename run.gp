
set xlabel 'N'
set ylabel 'Time(sec)'
set style fill solid
set title 'compute_pi time'
set term png enhanced font 'Verdana,10'
set output 'time.png'
set term pngcairo size 1280,960

plot "result_clock_gettime.csv" using 1:2 title 'baseline' with lines, \
     "result_clock_gettime.csv"	using 1:3 title 'openmp_2' with lines, \
     "result_clock_gettime.csv" using 1:4 title 'openmp_4' with lines

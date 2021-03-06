CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp
EXECUTABLE = \
	time_test_baseline time_test_openmp_2 time_test_openmp_4 \
	time_test_avx time_test_avxunroll \
	benchmark_clock_gettime

ifeq ($(strip $(AVX)),1)
CFLAGS += -mavx -DMAVX
endif

default: computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -o benchmark_clock_gettime -lm

avx-default: computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVX -o time_test_avx
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVXUNROLL -o time_test_avxunroll
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -DMAVX -o benchmark_clock_gettime -lm

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

check: default
	time ./time_test_baseline
	time ./time_test_openmp_2
	time ./time_test_openmp_4

avx: avx-default
	time ./time_test_baseline
	time ./time_test_openmp_2
	time ./time_test_openmp_4
	time ./time_test_avx
	time ./time_test_avxunroll

gencsv: default
	for i in `seq 100 5000 1000000`; do \
		printf "%d " $$i;\
		./benchmark_clock_gettime $$i; \
	done > result_clock_gettime.csv	

avx-gencsv: avx-default
	for i in `seq 103 5000 1000000`; do \
        printf "%d " $$i;\
        ./benchmark_clock_gettime $$i; \
    done > result_clock_gettime.csv 

plot: result_clock_gettime.csv
	gnuplot run.gp

avx-plot: result_clock_gettime.csv
	gnuplot avx_run.gp 

clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv

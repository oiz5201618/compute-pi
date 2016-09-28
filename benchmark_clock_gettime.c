#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define SAMPLE_TIME 1
#define M_PI acos(-1.0)

double compute_i(double time_pi[]);
double compute_error(double counted_pi);

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i;

    double compute_time[SAMPLE_TIME];

    // Baseline
    for(i = 0; i < SAMPLE_TIME;i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
	compute_time[i] = (double) (end.tv_sec - start.tv_sec) + 
			  (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf ", compute_i(compute_time));
    printf("%.15lf ", compute_error(compute_pi_baseline(N)));

    // OpenMP with 2 threads
    for(i = 0; i < SAMPLE_TIME;i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf ", compute_i(compute_time));
    printf("%.15lf ", compute_error(compute_pi_openmp(N, 2)));

    // OpenMP with 4 threads
    for(i = 0; i < SAMPLE_TIME;i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf ", compute_i(compute_time));
    printf("%.15lf \n", compute_error(compute_pi_openmp(N, 4)));

    //printf("%lf \n", compute_i(compute_time));

    // AVX SIMD
/*    clock_gettime(CLOCK_ID, &start);
    for(i = 0; i < loop; i++) {
        compute_pi_avx(N);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);


    // AVX SIMD + Loop unrolling
    clock_gettime(CLOCK_ID, &start);
    for(i = 0; i < loop; i++) {
        compute_pi_avx_unroll(N);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf\n", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);*/

    return 0;
}

double compute_i(double time_pi[]) {
    double mean = 0;
    double SD = 0; // Standard Deviation
    double Low_End, Up_End;
    double final_array[SAMPLE_TIME];
    double result = 0;
    int final_array_length = 0;

    int i;
    //compute mean
    for(i = 0; i < SAMPLE_TIME; i++) {
        mean += time_pi[i];
    }
    mean = mean / SAMPLE_TIME;

    //compute Standard Deviation
    for(i = 0; i < SAMPLE_TIME; i++) {
		SD += (time_pi[i] - mean) * (time_pi[i] - mean);
    }
    SD = SD / SAMPLE_TIME;
    SD = sqrt(SD);

    //find Lower-Endpoint & Upper-Endpoint
    Low_End = mean - 1.96 * SD;
    Up_End = mean + 1.96 * SD;

    //delete points out of range
    for(i = 0; i < SAMPLE_TIME; i++) {
		if(time_pi[i] >= Low_End && time_pi[i] <= Up_End) {
	    	final_array[final_array_length] = time_pi[i];
	    	final_array_length++;
		}
    }

    //compute result
    for(i = 0; i < final_array_length; i++) {
		result += final_array[i];
    }

    result = result / final_array_length;

    return result;

}

double compute_error(double counted_pi){
    double error = 0;

    if( counted_pi >= M_PI){
	error = (counted_pi - M_PI) / M_PI;
    }else{
	error = (M_PI - counted_pi) / M_PI;
    }

    return error;
}

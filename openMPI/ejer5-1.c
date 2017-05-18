#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void Find_bind(void);
int bin_count;
int main(int argc, char* argv[]){
        int thread_count= strtol(argv[1], NULL, 10);
	//input
	int max=100;
	int min=1;
	int  data_count=200;
	float data[200];
	bin_count=10;

	//output
	float bin_maxes[bin_count];
	int bin_counts[bin_count];

	int bin_width =(max-min)/bin_count;
	
	for (int b=0; b < bin_count; b++){
		bin_maxes[b]=min_meas+bin_width*(b+1)
	}

	for(int i=0; i< data_count; i++){
		bin = Find_bind(data[i], max, bin_count, min);
		bin_counts[bin]++;
	}

#       pragma omp parallel num_threads(thread_count)
        Hello();
        return 0;
}

void Find_bin(float data, float bin maxes, bin count, min meas){
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
	int count;
	
        printf("hello from thread %d of %d\n", my_rank, thread_count);
}

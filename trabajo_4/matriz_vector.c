#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

int *A, *x, *y;
int n, m;
int thread_count;

void *pth_mat_vect(void* rank){
	long my_rank= (long) rank;
	int i, j;
	int local_m = m/thread_count;
	int my_first_row = my_rank*local_m;
	int my_last_row = (my_rank+1)*local_m-1;

	for(i=my_first_row; i<=my_last_row; ++i){
		y[i]=0.0;
		for(j=0; j<n; ++j)
			y[i]+=A[i*n+j]*x[j];
	}

	return NULL;
}


int main(int argc, char* argv[]){
	int i,j;
	pthread_t* threads;
	clock_t start_t, end_t;
	double total_t;
	srand(time(NULL));
	thread_count=atoi(argv[1]);
	n=atoi(argv[2]);
	m=atoi(argv[3]);

	A = (int*) malloc(sizeof(int) * n*m);
	x = (int*) malloc(sizeof(int) * n);
	y = (int*) malloc(sizeof(int) * m);

	threads = (pthread_t*) malloc(sizeof(pthread_t) * thread_count);
	//Lleno matriz y vector
	for(i=0; i<m*n; ++i)
		A[i] = rand() % 100000;

	for(i=0; i<n; ++i)
		x[i]=rand() % 100000;
	start_t = clock();

	for(i=0; i<thread_count; ++i)
		pthread_create(&threads[i], NULL, pth_mat_vect, (void*) i);

	for( i = 0; i < thread_count; ++i)
		pthread_join(threads[i], NULL);
	end_t = clock();

	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("%f\n", total_t);
	return 0;
}

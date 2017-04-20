#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t head_p_mutex;
struct list_node_s* lista = NULL;
pthread_mutex_t list_mutex;
pthread_rwlock_t rwlock;

struct list_node_s{

	int data;
	struct list_node_s* next;
	pthread_mutex_t mutex;
};

int Member(int value, struct list_node_s* head_p){

	struct list_node_s* curr_p=head_p;

	while(curr_p != NULL && curr_p->data<value)
		curr_p = curr_p->next;

	if(curr_p == NULL || curr_p->data>value){
		return 0;
	} else {
		return 1;
	}
};

int sol2_Member( int value, struct list_node_s* head_pp){
	struct list_node_s* temp_p;

	pthread_mutex_lock(&head_p_mutex);
	temp_p = head_pp;
	while ( temp_p != NULL && temp_p->data < value) {
		if( temp_p->next != NULL ){
			pthread_mutex_lock(&(temp_p->next->mutex));
		}
		if (temp_p == head_pp) {
			pthread_mutex_unlock(&head_p_mutex);
		}
		pthread_mutex_unlock(&(temp_p->mutex));
		temp_p = temp_p->next;
	}
	if ( temp_p == NULL || temp_p->data > value ) {
		if( temp_p == head_pp ){
			pthread_mutex_unlock(&head_p_mutex);
		}
		if(temp_p != NULL){
			pthread_mutex_unlock(&(temp_p->mutex));
		}
		return 0;
	}
	else{
		if (temp_p == head_pp) {
			pthread_mutex_unlock(&head_p_mutex);
		}
		pthread_mutex_unlock(&(temp_p->mutex));
		return 1;
	}
};

int Insert(int value, struct list_node_s** head_pp ){

	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	struct list_node_s* temp_p;

	while ( curr_p != NULL && curr_p->data < value ) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if ( curr_p == NULL || curr_p->data > value) {
		temp_p = (struct list_node_s*) malloc(sizeof(struct list_node_s));
		temp_p->data = value;
		temp_p->next = curr_p;

		// printf("Inserto %d\n", value);

		if( pred_p == NULL ){
			*head_pp = temp_p;
		}
		else{
			pred_p->next = temp_p;
		}
		return 1;
	}
	else{
		return 0;
	}

};

int sol2_Insert(int value, struct list_node_s** head_pp ){

	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	struct list_node_s* temp_p;

	while ( curr_p != NULL && curr_p->data < value ) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if ( curr_p == NULL || curr_p->data > value) {
		temp_p = (struct list_node_s*) malloc(sizeof(struct list_node_s));
		temp_p->data = value;
		temp_p->next = curr_p;

		if( pred_p == NULL ){
			pthread_mutex_lock(&head_p_mutex);
			*head_pp = temp_p;
			pthread_mutex_unlock(&head_p_mutex);
		}
		else{
			pthread_mutex_lock(&(pred_p->mutex));
			pred_p->next = temp_p;
			pthread_mutex_unlock(&(pred_p->mutex));
		}
		return 1;
	}
	else{
		return 0;
	}

};


int Delete(int value, struct list_node_s** head_pp){
	
	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	
	while(curr_p != NULL && curr_p->data < value){
		
		pred_p=curr_p;
		curr_p=curr_p->next;
	}

	if(curr_p != NULL && curr_p->data==value){

		if(pred_p == NULL){
			
			*head_pp = curr_p->next;
			free(curr_p);
		} else {
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	} else {
		
		return 0;
	}
};

int sol2_Delete(int value, struct list_node_s** head_pp){

	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;

	while ( curr_p != NULL && curr_p->data < value ) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if( curr_p != NULL && curr_p->data < value ){
		if(pred_p == NULL){
			pthread_mutex_lock(&head_p_mutex);
			*head_pp = curr_p->next;
			free(curr_p);
			pthread_mutex_unlock(&head_p_mutex);
		}
		else{
			pthread_mutex_lock(&(pred_p->mutex));
			pred_p->next = curr_p->next;
			free(curr_p);
			pthread_mutex_unlock(&(pred_p->mutex));
		}
		return 1;
	}
	else{
		return 0;
	}
};

void Print(struct list_node_s** head_pp ){

	struct list_node_s* curr_p = *head_pp;

	while ( curr_p != NULL ) {
		printf("%d-", curr_p->data);
		curr_p = curr_p->next;
	}
	printf("\n");
};

void* Operaciones(void* id){
	int i = 0;
	int num[10000];

	for(i = 0; i <= 10000; ++i){
		num[i]=	rand() % 10000;
		sol2_Insert(rand() % 10000, &lista);
	}
	for(i = 0; i <= 80000; ++i){
		sol2_Member(rand() % 10000, lista);
	}
	for(i = 0; i <= 10000; ++i){
		sol2_Delete(num[rand() % 1000], &lista);
	}
	return 0;
}

int main(int argc, char* argv[]){

	srand(time(NULL));
	int n = atoi(argv[1]);
	long i = 0;

	clock_t start_t, end_t;
	double total_t;
	pthread_t* threads;
	threads = (pthread_t*) malloc(sizeof(pthread_t) * n);

	start_t = clock();

	for( i = 0; i < n; ++i){
		pthread_create(&threads[i], NULL, Operaciones, (void*) i);
	}
	for( i = 0; i < n; ++i){
		pthread_join(threads[i], NULL);
	}

	end_t = clock();

	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Total time: %f with %d threads.\n", total_t, n );


	return 0;
}

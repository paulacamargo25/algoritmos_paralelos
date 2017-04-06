#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

int *create_rand_nums(int num_elements) {
  int *rand_nums = (int *)malloc(sizeof(int) * num_elements);
  assert(rand_nums != NULL);
  int i;
  printf("Array\n");
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() % 100);
    printf("%i,", rand_nums[i]);
  }
  return rand_nums;
}

int compute_max(int *array, int num_elements) {
  int max = array[0];
  printf("max = %d", max);
  int i;
  for (i = 1; i < num_elements; i++) {
    if (array[i]>max){
      max = array[i];
    }
  }
  return max;
}

int main(int argc, char** argv) {

  int num_elements_per_proc = atoi(argv[1]);
  srand(time(NULL));
  int* rand_nums = NULL;

  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

  int *sub_rand_nums = (int *)malloc(sizeof(int) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_INT, sub_rand_nums,
              num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

  int sub_max = compute_max(sub_rand_nums, num_elements_per_proc);

  int *sub_maxs = NULL;
  if (world_rank == 0) {
    sub_maxs = (int *)malloc(sizeof(int) * world_size);
    assert(sub_maxs != NULL);
  }
  MPI_Gather(&sub_max, 1, MPI_INT, sub_maxs, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    int max = compute_max(sub_maxs, world_size);
    printf("\nMax of all elements is %i\n", max);
    int original_data_max =
      compute_max(rand_nums, num_elements_per_proc * world_size);
    printf("Max computed across original data is %i\n", original_data_max);
  }

  if (world_rank == 0) {
    free(rand_nums);
    free(sub_maxs);
  }

  free(sub_rand_nums);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}


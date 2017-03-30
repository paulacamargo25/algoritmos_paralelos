#include <stdio.h>
#include <stdlib.h>
#include<mpi.h>

int ping_pong(int in_val, int my_rank, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
    int p, my_rank;
    MPI_Comm comm;
    int in_val=0;
    double my_start, my_finish, my_elapsed;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &my_rank);

    if (my_rank == 0) {
        printf("Enter an int\n");
        scanf("%d", &in_val);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    my_start=MPI_Wtime();
    in_val=ping_pong(in_val, my_rank, p, comm);
    MPI_Barrier(MPI_COMM_WORLD);
    my_finish=MPI_Wtime();
    my_elapsed=my_finish-my_start;
    //printf("Proc %d > result = %d, time = %f\n", my_rank, result, my_elapsed);
    printf("Proc %d > result = %d, time = %f\n", my_rank, in_val, my_elapsed);

    MPI_Finalize();
    return 0;
}  /* main */


int ping_pong(int in_val, int my_rank, int p, MPI_Comm comm) {

    int n=0;
    printf("Proceso %d\n", my_rank);
    if(my_rank !=0){

        printf("REcibiendo...\n");
        MPI_Recv(&in_val, 1, MPI_INT, my_rank - 1 , 0, comm, MPI_STATUS_IGNORE);
        printf("REcibi\n");
        in_val+=1;
        MPI_Send(&in_val, 1, MPI_INT, (my_rank + 1)%2, 0, comm);
    }
    else {
        printf("Envio %d\n", in_val);
        MPI_Send(&in_val, 1, MPI_INT, my_rank+1, 0, comm);
    }

    while (n<5){
        MPI_Recv(&in_val, 1, MPI_INT, (my_rank + 1)%2  , 0, comm, MPI_STATUS_IGNORE);
        in_val+=1;
        MPI_Send(&in_val, 1, MPI_INT, (my_rank + 1)%2, 0, comm);
        ++n;
    }
    return in_val;

}  /* Bcast */

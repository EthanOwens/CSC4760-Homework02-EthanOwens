#include <mpi.h>
#include <stdio.h>

int main(int argc, cahr** argv){
    MPI_Init(&argc, &argv);

    int world_size, world_rank, token;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank != 0){
        MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Token %d received from process %d to process %d\n", token, world_rank - 1, world_rank);
    } else {
        token = -1;
    }

    MPI_Send(&token, 1, MPI_INT, (world_rank + 1) %world_size, 0, MPI_COMM_WORLD);

    if(world_rank == 0){
        MPI_Recv(&token, 1, MPI_INT, world_size - 1,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Token %d received from process %d to process %d\n", token, world_rank - 1, world_rank);
    }

    MPI_Finalize();
    return 0;
}
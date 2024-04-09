#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // P*Q must equal world_size
    int P = 1, Q = 1; 
    int color_1 = world_rank / Q;
    int color_2 = world_rank % Q;

    MPI_Comm split_comm_1;
    MPI_Comm_split(MPI_COMM_WORLD, color_1, world_rank, &split_comm_1);

    MPI_Comm split_comm_2;
    MPI_Comm_split(MPI_COMM_WORLD, color_2, world_rank, &split_comm_2);

    MPI_Comm_free(&split_comm_1);
    MPI_Comm_free(&split_comm_2);

    MPI_Finalize();
    return 0;
}
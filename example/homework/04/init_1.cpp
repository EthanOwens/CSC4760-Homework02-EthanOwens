#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("ranks %d \n", rank);
    
    int send_buf = rank + 1;
    int recv_buf;

    //braodcast value
    MPI_Bcast(&send_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //reduce value
    MPI_Reduce(&send_buf, &recv_buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    //broadcast reduced value
    MPI_Bcast(&recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("received value %d from rocess %d \n", recv_buf, rank);

    MPI_Finalize();
    return 0;
}
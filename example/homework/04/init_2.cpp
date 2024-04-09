#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_buf = rank + 1;
    int *recv_buf = (int *)malloc(size * sizeof(int));

    //Gather values
    MPI_Gather(&send_buf, 1, MPI_INT, recv_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //broadcast gathered values
    MPI_Bcast(recv_buf, size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Received values from process %d: ", rank);
    for(int i = 0; i < size; i++){
        printf("%d ", recv_buf[i]);
    }

    free(recv_buf);

    MPI_Finalize();
    return 0;
}
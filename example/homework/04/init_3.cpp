#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_buf = (int *)malloc(size * sizeof(int));
    int *recv_buf = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        send_buf[i] = rank * size + i;
    }

    MPI_Alltoall(send_buf, 1, MPI_INT, recv_buf, 1, MPI_INT, MPI_COMM_WORLD);

    printf("Received values from process %d: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", recv_buf[i]);
    }

    free(send_buf);
    free(recv_buf);

    MPI_Finalize();
    return 0;
}
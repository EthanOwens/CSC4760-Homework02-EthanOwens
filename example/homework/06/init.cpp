#include <Kokkos_Core.hpp>
#include <mpi.h>
#include <cstdio>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  Kokkos::initialize(argc, argv);
  {
  // Make View and init values
    int N = 20;
    Kokkos::View<double*> view("view", N);


    if(world_rank == 0){
      Kokkos::parallel_for(N, KOKKOS_LAMBDA(const int i) {
        view(i) = i;
      });
      Kokkos::fence();

    // Send View values with MPI functions

    MPI_Send(view.data(), N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    }else if(world_rank == 1){
      MPI_Recv(view.data(), N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
    }

  // Output
  printf("\nhello world\n");
  }
  Kokkos::finalize();
  MPI_Finalize();
}

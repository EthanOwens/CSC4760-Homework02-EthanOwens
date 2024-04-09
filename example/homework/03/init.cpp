#include <Kokkos_Core.hpp>
#include <cstdio>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    // Make View of length n > 1000 and add values
    const int N = 1000;
    Kokkos::View<int*> view_1("View_1", N);
    Kokkos::parallel_for("Init_View_1", N, KOKKOS_LAMBDA(const int i){
      view_1(i) = i * i;
    });

    // create two additional views of same size and datatype
    Kokkos::View<int*> view_2("View_2", N);
    Kokkos::View<int*> view_3("View_3", N);

    // deep_copy
    Kokkos::Timer timer_1;
    Kokkos::deep_copy(View_2, View_1);
    double time_1 = timer_1.seconds();

    // user copy
    Kokkos::Timer timer_2;
    Kokkos::parallel_for("User_Copy_View", N, KOKKOS_LAMBDA(const int i) {
        view_3(i) = view_1(i);
    });
    double time_2 = timer_2.seconds();

    // Output times 
    printf("deep_copy time: %f\n", time_1);
    printf("user copy time: %f\n", time_2);
  }
  Kokkos::finalize();
  return 0;
}

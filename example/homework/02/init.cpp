using namespace std;
#include <iostream>
#include <assert.h>
#include <string>

#include <mpi.h>

#define GLOBAL_PRINT

class Domain
{
public:
  Domain(int _M, int _N, const char *_name="") : domain(new char[(_M+1)*(_N+1)]), M(_M), N(_N), name(_name)  {}
  virtual ~Domain() {delete[] domain;}
  char &operator()(int i, int j) {return domain[i*N+j];}
  char operator()(int i, int j)const {return domain[i*N+j];}

  int rows() const {return M;}
  int cols() const {return N;}

  const string & myname() const {return name;}

  char *rawptr() {return domain;}
  
protected:
  char *domain; 
  int M;
  int N;

  string name;
};

void zero_domain(Domain &domain);
void print_domain(Domain &domain, int rank);
void update_domain(Domain &new_domain, Domain &old_domain, int size, int myrank, MPI_Comm comm);
void parallel_code(int M, int N, int iterations, int size, int myrank, MPI_Comm comm);

int main(int argc, char* argv[]){
    int M, N;
    int iterations;


    if(argc < 4){
        cout << "Not enough parameters" << endl;
        exit(0);
        }

    MPI_Init(&argc, &argv);

    int size, rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int array[3]; 
    if(rank == 0){
        M = atoi(argv[1]); 
        N = atoi(argv[2]); 
        iterations = atoi(argv[3]);

        array[0] = M;
        array[1] = N;
        array[2] = iterations;
    }

    MPI_Bcast(array, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank != 0){
        M = array[0];
        N = array[1];
        iterations = array[2];
    }

    parallel_code(M, N, iterations, size, rank, MPI_COMM_WORLD);

    MPI_Finalize();

}

void parallel_code(int M, int N, int iterations, int size, int myrank, MPI_Comm comm){

    int nominal = M / size; int extra = M % size;
    int m = (myrank < extra) ? (nominal+1) : nominal;
    int n = N; 

    Domain even_domain(m,n,"even Domain");
    Domain odd_domain(m,n,"odd Domain");

    zero_domain(even_domain);
    zero_domain(odd_domain);

    Domain *global_domain = nullptr;
    int *thecounts = nullptr;
    int *displs = nullptr;

    if(rank == 0){
        thecounts  = new int[size];
        displs = new int[size];


        for(int i = 0; i < size; ++i)
            thecounts[i] = nominal*n;  
        for(int i = 0; i < extra; ++i)
            thecounts[i] += n;   

        int count = displs[0] = 0;
        for(int i = 1; i < size; ++i){
            count += thecounts[i-1];
            displs[i] = count;
        }
    
        global_domain = new Domain(M,N,"Global Domain");
        zero_domain(*global_domain);

        if((n >= 8) && (m >= 10))
        {
            (*global_domain)(0,(n-1)) = 1;
            (*global_domain)(0,0)     = 1;
            (*global_domain)(0,1)     = 1;

            // and a glider:
            (*global_domain)(8,5)     = 1;
            (*global_domain)(8,6)     = 1;
            (*global_domain)(8,7)     = 1;
            (*global_domain)(7,7)     = 1;
            (*global_domain)(6,6)     = 1;
        }
  }
  
  // scatter the data:
  MPI_Scatterv(((rank==0) ? global_domain->rawptr() : nullptr),
	       thecounts, displs, MPI_CHAR,
	       even_domain.rawptr(), m*n, MPI_CHAR, 0, comm);
  
  if((n >= 8) && (m >= 10)){
    even_domain(0,(n-1)) = 1;
    even_domain(0,0)     = 1;
    even_domain(0,1)     = 1;

    even_domain(3,5) = 1;
    even_domain(3,6) = 1;
    even_domain(3,7) = 1;

    even_domain(6,7) = 1;
    even_domain(7,7) = 1;
    even_domain(8,7) = 1;
    even_domain(9,7) = 1;

    even_domain(0,(n-1)) = 1;
    even_domain(0,0)     = 1;
    even_domain(0,1)     = 1;

    even_domain(8,5)     = 1;
    even_domain(8,6)     = 1;
    even_domain(8,7)     = 1;
    even_domain(7,7)     = 1;
    even_domain(6,6)     = 1;
  
  }


    if(0 == rank){  
      cout << "Initial State:" << endl;
      print_domain(*global_domain, myrank);
    }

    cout << "Initial State:" << i << endl;
    print_domain(*even, rank);


  Domain *odd, *even; // pointer swap magic
  odd = &odd_domain;
  even = &even_domain;

  for(int i = 0; i < iterations; ++i){
    update_domain(*odd, *even, size, rank, comm);

    if(0 == myrank){
      cout << "Iteration #" << i << endl;
  }

    // gather all data back into the global domain in process rank 0:
    MPI_Gatherv(odd->rawptr(), m*n, MPI_CHAR,
		((myrank==0) ? global_domain->rawptr() : nullptr),
		thecounts, displs, MPI_CHAR, 0, comm);

    if(0 == rank)
      print_domain(*global_domain, myrank);

    cout << "Iteration #" << i << endl; 
    print_domain(*odd, rank);


    // swap pointers:
    Domain *temp = odd;
    odd  = even;
    even = temp;
  }

  if(0 == rank){
    delete global_domain;
    delete[] thecounts;
    delete[] displs;
  }
}
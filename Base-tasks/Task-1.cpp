// mpic++ Task-1.cpp
// mpirun -np 4 ./a.out

#include "../Manager.h"

#include <iostream>

auto main(int argc, char *argv[]) -> decltype(argc) {
  MPIManager MPI;
  std::cout << "Hello world [rank: " << MPI.getPID() 
  << ", group size: " << MPI.getMPIGroupSize() << "]" << std::endl;
}
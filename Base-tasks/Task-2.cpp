//mpic++ Task-2.cpp -std=c++1z  
//mpirun -np 6 ./a.out 40    
#include "../Manager.h"

#include <stdlib.h>
#include <iostream>
#include <tuple>


class Sender {

  double getMainWork(int From, int To) const{
    auto Res = 0.0;
    assert(From > 0 && "Incorerct range");
    auto CurPos = From + 0.0;
    for (; CurPos < To; CurPos++) {
      Res += 1 / CurPos;
    }
    return Res;
  }

  std::tuple<int, int> getRange() const {
    auto NumOfSenders = MPI.getMPIGroupSize();
    auto MainWork = static_cast<int>(N / NumOfSenders);
    auto From = MPI.getPID() * MainWork + 1; //because N != 0
    return std::make_tuple(From, From + MainWork);
  }

  double getAdditionalWork() const {
    auto NumOfSenders = MPI.getMPIGroupSize();
    auto AdditionalWork = N % NumOfSenders;
    auto MainWorkSize = static_cast<int>(N / NumOfSenders) * NumOfSenders;
    if (MPI.getPID() < AdditionalWork)
      return 1 / (MainWorkSize + MPI.getPID() + 1.0); //because N != 0
    return 0;
  }

public:
  
  Sender(MPIManager& MPIMan, int NumOfSenders) : MPI{MPIMan},
                                                 N{NumOfSenders} {}
  
  double getWork() const {
    auto[From, To] = getRange();
    auto Res = getMainWork(From, To);
    Res += getAdditionalWork();
    return Res;
  }

  void sendResult() const {
    auto Result = getWork();
    MPI.send(Result, 0);
  }

private:
  MPIManager& MPI;
  int N;
};


class Receiver {

  double getOtherWork() const {
    auto NumOfOtherSenders = MPI.getMPIGroupSize() - 1;
    auto Res = 0.0;
    for (; NumOfOtherSenders > 0; NumOfOtherSenders--)
      Res += MPI.recv<double>(NumOfOtherSenders);
    return Res;
  }

  double getCurWork() const {
    Sender This{MPI, N};
    return This.getWork();
  }

public:

  Receiver (MPIManager& MPIMan, int NumOfSenders) : MPI{MPIMan},
                                                    N{NumOfSenders} {}
  
  double getResult() const {
    auto Res = getOtherWork();
    Res += getCurWork();
    return Res;
  }

private:
  MPIManager& MPI;
  int N;
};


auto main(int argc, char** argv) -> int {
  MPIManager MPI;
  assert(argc > 1 && "N doesn't givven");
  auto N = atoi(argv[1]);
  std::cout << "Hello from " << MPI.getPID() << std::endl;
  if (MPI.getPID() == 0) {
    Receiver Rec{MPI, N};
    std::cout << "Result: " << Rec.getResult() << std::endl;
    return 0;
  }
  Sender Send{MPI, N};
  Send.sendResult();
}
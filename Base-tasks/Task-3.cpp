#include "../Manager.h"

#include <iostream>
#include <string>
#include <sstream>
#include <array>

class Sender {

public:
  
  Sender(MPIManager& MPIMan) : MPI{MPIMan} {}
  
  template <typename T>
  T getNewMsg(T OldMsg) {
    return OldMsg + 1;
  }

  void sendToNext(int Msg) {
    auto DestID = (MPI.getPID() + 1) % MPI.getMPIGroupSize();   
    MPI.send(getNewMsg(Msg), DestID);
  }

  int recvFromPrev() {
    auto SrcID = MPI.getPID() ? (MPI.getPID() - 1) % MPI.getMPIGroupSize() 
                              : MPI.getMPIGroupSize() - 1;
    return MPI.recv<int>(SrcID);
  }

private:
  MPIManager& MPI;
};

auto main(int argc, char** argv) -> decltype(1 + 1) {
  MPIManager MPI;
  Sender Send{MPI};
  if (MPI.getPID() == 0)
    Send.sendToNext(0);
  auto Msg = Send.recvFromPrev();
  if (MPI.getPID() != 0) {
    Send.sendToNext(Msg);
  } else {
    std::cout << "Msg: " << Msg << "\nFinal hello from " << MPI.getPID() << std::endl;
  }
}
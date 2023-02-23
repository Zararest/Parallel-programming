#ifndef MANAGER_H
#define MANAGER_H

#include <mpi.h>
#include <stdio.h>
#include <utility>
#include <cassert>
#include <algorithm>
#include <string>

#define DEFAULT_TAG 2

using PID = unsigned;

enum class MPIGroup {
  World
};

MPI_Comm getGroup(MPIGroup Group) {
  switch (Group) {
  case MPIGroup::World:
    return MPI_COMM_WORLD;
  default:
    assert(false && "Group is unsupported");
  }
} 

class MPIManager  {

public: 

  MPIManager() {
    MPI_Init(nullptr, nullptr);
  }

  MPIManager(const MPIManager&) = delete;
  MPIManager(MPIManager&&) = default;

  ~MPIManager() {
    MPI_Finalize();
  }

  size_t getMPIGroupSize(MPIGroup Group = MPIGroup::World) const {
    auto Rank = 0;
    MPI_Comm_size(getGroup(Group), &Rank);
    return Rank;
  }

  PID getPID(MPIGroup Group = MPIGroup::World) const{
    auto Size = 0;
    MPI_Comm_rank(getGroup(Group), &Size);
    return Size;
  }

  template <typename T>
  void send(T Msg, PID Dest, int MsgTag = DEFAULT_TAG, 
            MPI_Comm Group = getGroup(MPIGroup::World)) const {
    auto Size = sizeof(T);
    #ifdef DEBUG 
    std::cerr << "Send: " << Msg << std::endl; 
    #endif
    auto Ret = MPI_Send(reinterpret_cast<char*>(&Msg), Size, MPI_CHAR, Dest,
                        MsgTag, Group); //0 - OK
  }

  template <typename T>
  T recv(PID Src, int MsgTag = DEFAULT_TAG,
         MPI_Comm Group = getGroup(MPIGroup::World)) const {
    auto Size = sizeof(T);
    char Buf[Size] = {};
    MPI_Status Status;
    auto Ret = MPI_Recv(Buf, Size, MPI_CHAR, Src,
                        MsgTag, Group, &Status);
    auto Res = *reinterpret_cast<T*>(Buf);
    #ifdef DEBUG
    std::cerr << "Recv: " << Res << std::endl;
    #endif 
    return Res;
  }
};

template <>
void MPIManager::send(std::string Msg, PID Dest, int MsgTag, 
          MPI_Comm Group) const {

}

template <>
std::string MPIManager::recv(PID Src, int MsgTag,
        MPI_Comm Group) const {
}

#endif
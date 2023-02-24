//mpirun -np 4 ./e-calc > ../result

#include <boost/multiprecision/gmp.hpp> 
#include <vector>
#include <iostream>

#include "Manager.h"
#include "gmpxx.h"

#define SERIES_SIZE 1000000
#define ITER_SIZE 10


constexpr unsigned getPrecision(){ return 1000; }
constexpr unsigned FloatSize = getPrecision();
constexpr unsigned IntSize = getPrecision();

using BigFloat = boost::multiprecision::number<boost::multiprecision::backends::gmp_float<FloatSize>>;
using BigInt = boost::multiprecision::mpz_int;

class Worker {

  unsigned getPrevWorkerID() {
     return MPI.getPID() ? (MPI.getPID() - 1) % MPI.getMPIGroupSize() 
                                               :  MPI.getMPIGroupSize() - 1;
  }

  unsigned getNextWorkerID() {
    return (MPI.getPID() + 1) % MPI.getMPIGroupSize();
  }

  BigFloat getAdditionalWork() {
    assert(MPI.getPID() == 0);
    auto AdditionalSeqBeg = (SizeOfSeq * NumOfWorkers) * NumOfIter + 1;
    BigFloat AdditionalRes = 1, CurFactorial = 1;
    #ifdef DEBUG_GROUP
    std::cout << "Additional: {" << AdditionalSeqBeg << ", " << N << "}\n";
    #endif
    for (unsigned SeqPos = 0; AdditionalSeqBeg + SeqPos <= N; SeqPos++) {
      CurFactorial = CurFactorial / (AdditionalSeqBeg + SeqPos);
      AdditionalRes += CurFactorial;
    }
    return AdditionalRes * recvRes(getPrevWorkerID());
  }

  BigFloat recvRes(unsigned SrcID) {
    auto PrevFactorial =  MPI.recv<std::string>(SrcID, DEFAULT_TAG, getGroup(MPIGroup::World));
    return BigFloat{PrevFactorial};
  }

  void sendCurRes(BigFloat Res, 
                  unsigned DestID) {
    MPI.send(Res.str(), DestID, DEFAULT_TAG, getGroup(MPIGroup::World));
  }

  BigFloat calcCachedWork() {
    BigFloat Res = 0;
    for (auto It : CachedWork)
      Res = Res + It;
    return Res; 
  }

  BigFloat getAllMainWork() {
    BigFloat Res = 0;
    for (auto WorkerId = 1; WorkerId < MPI.getMPIGroupSize(); WorkerId++)
      Res = Res + recvRes(WorkerId);
    return Res + calcCachedWork();
  }

  void sendCachedWork() {
    if (MPI.getPID() == 0)
      return;
    sendCurRes(calcCachedWork(), 0);
  }

  void calcCurGroup(unsigned CurIt) {
    auto WorkerID = MPI.getPID();
    auto AllWorkPerIter = NumOfWorkers * SizeOfSeq;
    auto CurSeqBegin = CurIt * AllWorkPerIter + WorkerID * SizeOfSeq + 1;
    BigFloat SeqRes = 0, CurFactorial = 1;
    #ifdef DEBUG_GROUP
    std::cout << "Group: {" << CurSeqBegin << ", " << CurSeqBegin + SizeOfSeq - 1 << "}\n"; 
    #endif
    for (unsigned SeqPos = 0; SeqPos < SizeOfSeq; SeqPos++) {
      CurFactorial = CurFactorial / (CurSeqBegin + SeqPos);
      SeqRes += CurFactorial;
    }  
    auto PrevFactorial = recvRes(getPrevWorkerID());
    CachedWork.push_back(PrevFactorial * SeqRes);
    sendCurRes(PrevFactorial * CurFactorial, getNextWorkerID()); 
  }

public:

  Worker(MPIManager& MPIMan, unsigned SeriesSize,
         unsigned SizeOfIt) : MPI{MPIMan},
                              N{SeriesSize},
                              SizeOfSeq{SizeOfIt} {
    NumOfWorkers = MPIMan.getMPIGroupSize();
    NumOfIter = SeriesSize / (SizeOfIt * NumOfWorkers);
  }

  void start() {
    if (MPI.getPID() == MPI.getMPIGroupSize() - 1) 
      sendCurRes(BigFloat{"1"}, 0);
  }

  void calc() {
    for (unsigned CurIt = 0; CurIt < NumOfIter; CurIt++) 
      calcCurGroup(CurIt);
    sendCachedWork();
  } 

  BigFloat getReult() {
    assert(MPI.getPID() == 0);
    auto Main = getAllMainWork();
    auto Additional = getAdditionalWork();
    std::cout << "Main:" << Main << " Aditional: " << Additional << std::endl;
    return Main + Additional;
  }

private:
  std::vector<BigFloat> CachedWork;
  unsigned SizeOfSeq = 0;     //work per iteration per worker
  unsigned NumOfIter = 0;     //number of sequences
  unsigned NumOfWorkers = 1;    
  MPIManager& MPI;
  unsigned N;                 //number of elements in e series
};

int main() {
  MPIManager MPI;
  Worker Worker{MPI, SERIES_SIZE, ITER_SIZE};
  Worker.start();
  Worker.calc();
  if (MPI.getPID() == 0) {
    BigFloat Res{"1"};
    Res += Worker.getReult();
    std::cout << Res.str() << std::endl;
  }
}

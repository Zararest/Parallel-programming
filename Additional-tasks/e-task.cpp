#include <boost/multiprecision/gmp.hpp> 
#include <vector>

#include "Manager.h"
#include "gmpxx.h"

constexpr unsigned getPrecision(){ return 20; }
constexpr unsigned FloatSize = getPrecision();
constexpr unsigned IntSize = getPrecision();

using BigFloat = boost::multiprecision::number<boost::multiprecision::backends::gmp_float<FloatSize>>;
using BigInt = boost::multiprecision::mpz_int;

class Worker {

  BigInt getAdditionalWork() {
    assert(MPI.getPID() == 0);
    auto AdditionalSeqBeg = SizeOfSeq * NumOfIter + 1;
    BigInt AdditionalRes = 1;
    for (unsigned SeqPos = 0; AdditionalSeqBeg + SeqPos <= N; SeqPos++)
      AdditionalRes = AdditionalRes * (AdditionalSeqBeg + SeqPos);
    return AdditionalRes * recvRes();
  }

  BigInt recvRes(unsigned SrcID = MPI.getPID() ? (MPI.getPID() - 1) % MPI.getMPIGroupSize() 
                                               : MPI.getMPIGroupSize() - 1) {
    auto PrevFactorial =  MPI.recv<std::string>(SrcID, DEFAULT_TAG, getGroup(MPIGroup::World));
    return BigInt{PrevFactorial};
  }

  void sendCurRes(BigInt Res, 
                  unsigned DestID = (MPI.getPID() + 1) % MPI.getMPIGroupSize()) {
    MPI.send(Res.str(), DestId, DEFAULT_TAG, getGroup(MPIGroup::World));
  }

  BigInt calcCachedWork() {
    BigInt Res = 0;
    for (auto It : CachedWork)
      Res = Res + It;
    return Res; 
  }

  BigInt getAllMainWork() {
    BigInt Res = 0;
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
    BigInt SeqRes = 1;
    for (unsigned SeqPos = 0; SeqPos < SizeOfSeq; SeqPos++) 
      SeqRes = SeqRes * (CurSeqBegin + SeqPos);
    sendCurRes(recvRes() * SeqRes); 
  }

public:

  Worker(MPIManager& MPIMan, unsigned SeriesSize,
         unsigned SizeOfIt) : MPI{MPIMan},
                              N{SeriesSize},
                              SizeOfSeq{SizeOfIt} {
    NumOfIter = SeriesSize / SizeOfIt;
    NumOfWorkers = MPIMan.getMPIGroupSize();
  }

  void calc() {
    for (unsigned CurIt = 0; CurIt < NumOfIter; CurIt++) 
      calcCurGroup(CurIt);
    sendCachedWork();
  } 

  BigFloat getReult() {
    assert(MPI.getPID() == 0);
    return getAllMainWork() + getAdditionalWork();
  }

private:
  std::vector<BigInt> CachedWork;
  unsigned SizeOfSeq = 0;     //work per iteration per worker
  unsigned NumOfIter = 0;     //number of sequences
  unsigned NumOfWorkers = 1;    
  MPIManager& MPI;
  unsigned N;                 //number of elements in e series
};


int main() {
  BigFloat f{"1.1232342342342342423423423442"};
  BigInt i{"123123131312313123123123123123131231312313123131231332312"};

  std::cout << "Float:" << f.str(10000) << std::endl;
  std::cout << "Size: " << i.str(1000) << std::endl;
}

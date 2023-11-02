#pragma once
#include <mpi.h>

class LeaderElectionAlgorithm {
public:
    LeaderElectionAlgorithm(int argc, char** argv);
    ~LeaderElectionAlgorithm();
    int getRank();
    int getSize();
    MPI_Comm getCommunicator();
    int electLeader();
private:
    MPI_Comm communicator;
    int rank, size, diameter;
    virtual void initFloodMax() = 0;
    virtual int sendCurrentMax(int max_uid, int round) = 0;
    virtual void receiveMax(int* max_uid, int round) = 0;
};

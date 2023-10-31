#pragma once

class LeaderElectionAlgorithm {
public:
    LeaderElectionAlgorithm(int argc, char** argv);
    ~LeaderElectionAlgorithm();
    int getRank();
    int getSize();
    int electLeader();
private:
    int rank, size;
    virtual void initFloodMax() = 0;
    virtual int sendCurrentMax(int max_uid, int round) = 0;
    virtual void receiveMax(int* max_uid, int round) = 0;
};

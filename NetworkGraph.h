#pragma once
#include <vector>
#include <mpi.h>

class NetworkGraph {
public:
    // void readGraph(const char* path);
    void generateGraph(MPI_Comm communicator);
    MPI_Comm initGraph(MPI_Comm old_communicator);
    int getDiameter();
private:
    int nnodes, diameter;
    std::vector<int> degrees;
    std::vector<int> edges;
};
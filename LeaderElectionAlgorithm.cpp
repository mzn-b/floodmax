#include "LeaderElectionAlgorithm.h"
#include <iostream>
#include <mpi.h>

LeaderElectionAlgorithm::LeaderElectionAlgorithm(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
}

LeaderElectionAlgorithm::~LeaderElectionAlgorithm() {
    MPI_Finalize();
}

int LeaderElectionAlgorithm::getRank() {
    return rank;
}

int LeaderElectionAlgorithm::getSize() {
    return size;
}

int LeaderElectionAlgorithm::electLeader() {

    const int diam = size;  // assume worst case
    int max_uid = rank;
    int messageCount = 0;

    // Execute FloodMax algorithm
    initFloodMax();
    for (int round = 0; round < diam; round++)
    {
        messageCount += sendCurrentMax(max_uid, round);
        receiveMax(&max_uid, round);
    }

    // Print leadership status
    if (max_uid == rank)
    {
        std::cout << "Process " << rank << " is the leader." << std::endl;
    }
    else
    {
        std::cout << "Process " << rank << " is a non-leader." << std::endl;
    }

    // Elected leader aggregates message count and outputs it
    int totalMessages = 0;
    MPI_Reduce(&messageCount, &totalMessages, 1, MPI_INT, MPI_SUM, max_uid, MPI_COMM_WORLD);
    if (rank == max_uid)
    {
        std::cout << totalMessages << " messages sent in total" << std::endl;
    }

    return max_uid;

}
#include <iostream>
#include <mpi.h>
#include "LeaderElectionAlgorithm.h"
#include "NetworkGraph.h"

LeaderElectionAlgorithm::LeaderElectionAlgorithm(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    communicator = MPI_COMM_WORLD;
    int orig_rank, orig_size;
    MPI_Comm_rank(communicator, &orig_rank);
    MPI_Comm_size(communicator, &orig_size);

    // Generate a network graph where the first half of the nodes is fully connected
    // and the second half of the graph is fully connected, but the two subgraphs
    // are only connected by one node each
    NetworkGraph network;
    network.generateGraph(communicator);
    communicator = network.initGraph(communicator);
    diameter = network.getDiameter();

    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    printf("Process %i of %i has now rank %i of %i\n", orig_rank, orig_size, rank, size);
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

MPI_Comm LeaderElectionAlgorithm::getCommunicator() {
    return communicator;
}

int LeaderElectionAlgorithm::electLeader() {

    int max_uid = rank;
    int messageCount = 0;

    // Execute FloodMax algorithm
    initFloodMax();
    for (int round = 0; round < diameter; round++)
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
    MPI_Reduce(&messageCount, &totalMessages, 1, MPI_INT, MPI_SUM, max_uid, communicator);
    if (rank == max_uid)
    {
        std::cout << totalMessages << " messages sent in total" << std::endl;
    }

    return max_uid;

}
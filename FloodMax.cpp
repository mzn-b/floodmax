#include <mpi.h>
#include "LeaderElectionAlgorithm.h"

class FloodMax : public LeaderElectionAlgorithm {
public:
    FloodMax(int argc, char** argv) : LeaderElectionAlgorithm(argc, argv) {
    }

private:

    void initFloodMax() override {
        // nothing to do
    }

    int sendCurrentMax(int max_uid, int round) override {
        int messages = 0;
        for (int i = 0; i < getSize(); i++)
        {
            if (i != getRank())
            {
                MPI_Send(&max_uid, 1, MPI_INT, i, round, MPI_COMM_WORLD);
                messages++;
            }
        }
        return messages;
    }

    void receiveMax(int* max_uid, int round) override {
        for (int i = 0; i < getSize() - 1; i++)
        {
            int received_max;
            MPI_Recv(&received_max, 1, MPI_INT, MPI_ANY_SOURCE, round, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (received_max > *max_uid) *max_uid = received_max;
        }
    }

};

int main(int argc, char **argv)
{
    FloodMax algorithm(argc, argv);
    algorithm.electLeader();
}

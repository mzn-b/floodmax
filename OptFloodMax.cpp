#include <mpi.h>
#include <unistd.h>
#include <algorithm>
#include "LeaderElectionAlgorithm.h"

class OptFloodMax : public LeaderElectionAlgorithm {
public:
    OptFloodMax(int argc, char** argv) : LeaderElectionAlgorithm(argc, argv) {
    }

private:
    bool new_info;

    void initFloodMax() override {
        new_info = true;
    }

    int sendCurrentMax(int max_uid, int round) override {
        // std::cout << "Process " << getRank() << " is sending in round " << round << std::endl;
        if (!new_info) return 0;
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
        // std::cout << "Process " << getRank() << " is receiving in round " << round << std::endl;
        // receive asynchronously because we don't know how many messages we will receive
        int world_size = getSize();
        MPI_Request r[world_size - 1];
        MPI_Status statuses[world_size - 1];
        int recv_uids[world_size - 1];
        for (int i = 0; i < world_size - 1; i++)
        { // We expect to receive world_size-1 messages (everyone except ourselves)
            recv_uids[i] = -1;
            MPI_Irecv(&recv_uids[i], 1, MPI_INT, MPI_ANY_SOURCE, round, MPI_COMM_WORLD, &r[i]);
        }

        // wait for 1 second to receive messages for this round
        sleep(1);
        int received[world_size - 1];
        for (int i = 0; i < world_size - 1; i++)
        {
            MPI_Test(&r[i], &received[i], &statuses[i]);
            if (!received[i])
                MPI_Cancel(&r[i]);
        }

        // determine maximum
        int received_max = *std::max_element(recv_uids, recv_uids + world_size-1);
        if (received_max > *max_uid) {
            *max_uid = received_max;
            new_info = true;
        } else {
            new_info = false;
        }
        std::cout << "Process " << getRank() << " received " << *max_uid << " in round " << round << std::endl;
    }

};

int main(int argc, char **argv)
{
    OptFloodMax algorithm(argc, argv);
    algorithm.electLeader();
}
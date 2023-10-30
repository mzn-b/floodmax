#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <sstream>
#include <unistd.h>

int main(int argc, char **argv)
{
    int world_size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int diam = world_size; // In a real-world application, this might not be equal to world_size
    int max_uid = rank;
    int rounds = 0;
    bool new_info = true;

    int messageCount = 0; // Counter for the number of messages

    while (rounds < diam)
    {
        std::cout << "Process " << rank << " entering round " << rounds << std::endl;
        // std::cout << "Sending from " << rank << " in round " << rounds << std::endl;
        // Sending phase>
        if (new_info)
        {
            for (int i = 0; i < world_size; i++)
            {
                if (i != rank)
                {
                    MPI_Send(&max_uid, 1, MPI_INT, i, rounds, MPI_COMM_WORLD);
                    messageCount++;
                }
            }
        }

        // std::cout << "Receiving at " << rank << " in round " << rounds << std::endl;
        // Receiving phase
        MPI_Request r[world_size - 1];
        MPI_Status statuses[world_size - 1];
        int recv_uids[world_size - 1];
        for (int i = 0; i < world_size - 1; i++)
        { // We expect to receive world_size-1 messages (everyone except ourselves)
            recv_uids[i] = -1;
            MPI_Irecv(&recv_uids[i], 1, MPI_INT, MPI_ANY_SOURCE, rounds, MPI_COMM_WORLD, &r[i]);
        }

        sleep(1);
        int received[world_size - 1];
        for (int i = 0; i < world_size - 1; i++)
        {
            MPI_Test(&r[i], &received[i], &statuses[i]);
            if (!received[i])
                MPI_Cancel(&r[i]);
        }

        // Update max_uid
        int old_max_uid = max_uid;
        max_uid = std::max(max_uid, *std::max_element(recv_uids, recv_uids + world_size - 1));
        // std::cout << "Receiving max_uid as " << max_uid << " at process " << rank << std::endl;

        // Update new_info flag
        new_info = old_max_uid != max_uid;

        rounds++;
    }

    // Determine leader status
    std::ostringstream oss;
    if (max_uid == rank)
    {
        oss << "Process " << rank << " is the leader." << std::endl;
    }
    else
    {
        oss << "Process " << rank << " is a non-leader." << std::endl;
    }
    std::cout << oss.str();

    // Gather all the message counts at rank 0
    int totalMessages = 0;
    MPI_Reduce(&messageCount, &totalMessages, 1, MPI_INT, MPI_SUM, max_uid, MPI_COMM_WORLD);

    // Rank 0 prints the total count
    if (rank == max_uid)
    {
        std::cout << totalMessages << " messages sent in total" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

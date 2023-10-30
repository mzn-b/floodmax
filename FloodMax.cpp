#include <iostream>
#include <sstream>
#include <vector>
#include <mpi.h>

const int UID_TAG = 1;

int main(int argc, char **argv)
{
    int rank, size;
    int sent_messages_local = 0;
    int sent_messages_global = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Assuming `diam` is known and is a constant for simplicity.
    const int diam = size; // This is a placeholder, replace with actual value.

    int u = rank; // UID initially set to the rank.
    int max_uid = u;
    int rounds = 0;
    int status = 0; // 0: unknown, 1: leader, 2: non-leader

    while (rounds < diam)
    {
        // Send max_uid to all other processes.
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                MPI_Send(&max_uid, 1, MPI_INT, i, UID_TAG, MPI_COMM_WORLD);
                sent_messages_local += size-1;
            }
        }

        // Receive UIDs from all other processes and update max_uid.
        for (int i = 0; i < size - 1; i++)
        {
            int received_uid;
            MPI_Recv(&received_uid, 1, MPI_INT, MPI_ANY_SOURCE, UID_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (received_uid > max_uid)
            {
                max_uid = received_uid;
            }
        }

        rounds++;
    }

    std::ostringstream oss;
    if (max_uid == u)
    {
        status = 1;
        oss << "Process " << rank << " is the leader." << std::endl;
    }
    else
    {
        status = 2;
        oss << "Process " << rank << " is a non-leader." << std::endl;
    }
    std::cout << oss.str();

    MPI_Reduce(&sent_messages_local, &sent_messages_global, 1, MPI_INT, MPI_SUM, max_uid, MPI_COMM_WORLD);
    if (status == 1) {
        std::cout << "Sent " << sent_messages_global << " messages in total" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

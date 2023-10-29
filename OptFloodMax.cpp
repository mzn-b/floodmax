#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int diam = world_size; // In a real-world application, this might not be equal to world_size
    int u = rank;
    int max_uid = u;
    int rounds = 0;
    bool new_info = true;

    while (rounds < diam) {
        std::vector<int> received_uids;

        // Sending phase
        if (new_info) {
            for (int i = 0; i < world_size; i++) {
                if (i != rank) {
                    MPI_Send(&max_uid, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }
        }

        // Receiving phase
        for (int i = 0; i < world_size - 1; i++) {  // We expect to receive world_size-1 messages (everyone except ourselves)
            int recv_uid;
            MPI_Recv(&recv_uid, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            received_uids.push_back(recv_uid);
        }

        // Update max_uid
        int old_max_uid = max_uid;
        max_uid = std::max(max_uid, *std::max_element(received_uids.begin(), received_uids.end()));

        // Update new_info flag
        new_info = old_max_uid != max_uid;

        rounds++;
    }

    // Determine leader status
    if (max_uid == u) {
        std::cout << "Process " << rank << " is the leader!" << std::endl;
    } else {
        std::cout << "Process " << rank << " is not the leader." << std::endl;
    }

    MPI_Finalize();
    return 0;
}

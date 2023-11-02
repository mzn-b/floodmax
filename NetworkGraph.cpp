#include <string>
#include "NetworkGraph.h"

/*
void NetworkGraph::readGraph(const char *path)
{
    auto fid = fopen(path, "r");
    fscanf(fid, "Nodes: %i\n", &nnodes);
    std::vector<int> edgeBuffer(nnodes, -1);
    for (int i = 0; i < nnodes; i++) {
        edgeBuffer.clear();
        // TODO: Read edges from node i
        // ...
        
        //std::getline(fid, line, '\n');
        // update degrees and edges
        int degree = edgeBuffer.size();
        degrees.push_back(degree);
        for (int j = 0; j < degree; j++)
        {
            edges.push_back(edgeBuffer[j]);
        }
    }
    fscanf(fid, "Diameter: %i", &diameter);
    fclose(fid);
}
*/

void NetworkGraph::generateGraph(MPI_Comm communicator) {
    MPI_Comm_size(communicator, &nnodes);
    degrees.clear();
    edges.clear();
    int split = nnodes / 2;

    // fully connect first half of nodes
    for (int from = 0; from < split; from++) {
        degrees.push_back(0);
        for (int to = 0; to < split; to++) {
            if (to == from) continue;
            degrees[from] += 1;
            edges.push_back(to);
        }
    }

    // connect last node of first half to first node of second half
    degrees[split-1] += 1;
    edges.push_back(split);
    degrees.push_back(1);
    edges.push_back(split-1);

    // fully connect second half of nodes
    for (int from = split; from < nnodes; from++) {
        if (from > split) degrees.push_back(0);
        for (int to = split; to < nnodes; to++) {
            if (to == from) continue;
            degrees[from] += 1;
            edges.push_back(to);
        }
    }
    diameter = 3;
}

int NetworkGraph::getDiameter() {
    return diameter;
}

MPI_Comm NetworkGraph::initGraph(MPI_Comm old_communicator)
{
    //printf("Degrees: %i %i %i %i %i %i\n", degrees[0], degrees[1], degrees[2], degrees[3], degrees[4], degrees[5]);
    MPI_Comm graphCommunicator;
    MPI_Graph_create(old_communicator, nnodes, degrees.data(), edges.data(), 1, &graphCommunicator);
    return graphCommunicator;
}
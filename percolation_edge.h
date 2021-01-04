#ifndef PERCOLATION_EDGE_ADD_H
#define PERCOLATION_EDGE_ADD_H
#include <graph.h>
#include <unionfind.h>
#include <vector>
#include <percolation.h>

class Percolation_edge : public Percolation {
public:
    Percolation_edge();
    Percolation_edge(unsigned long int number_of_checkpoints);
    void percolate(Graph &G);
    std::vector<double> get_result();
    std::vector<double> get_other_result();
};

#endif // PERCOLATION_EDGE_ADD_H

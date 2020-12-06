#ifndef PERCOLATION_EDGE_ADD_H
#define PERCOLATION_EDGE_ADD_H
#include <graph.h>
#include <unionfind.h>
#include <vector>

class Percolation_edge {
private:
    unsigned long int noc;
    std::vector<std::vector<double>> result;

public:
    Percolation_edge(unsigned long int number_of_checkpoints);
    void percolation_molloy_reed_criterion(Graph & g);
    std::vector<std::vector<double>> get_result();
};

#endif // PERCOLATION_EDGE_ADD_H

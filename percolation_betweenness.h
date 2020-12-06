#ifndef BETWEENNESS_PERCOLATION_H
#define BETWEENNESS_PERCOLATION_H
#include <vector>
#include <graph.h>
#include <heap_desc.h>
#include <betweenness.h>
#include <unionfind.h>
#include <thread>

class Percolation_betweenness {
private:
    double mean_l;
    std::vector<double> result;
    void remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix);
    unsigned long int get_biggest(std::vector<std::vector<unsigned long int>> & adj_matrix);
public:
    Percolation_betweenness();
    void percolation_betweenness(Graph &G, unsigned long int n_threads);
    std::vector<double> get_result();
    double get_mean_l();
};

#endif // BETWEENNESS_PERCOLATION_H

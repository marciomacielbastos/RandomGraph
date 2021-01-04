#ifndef BETWEENNESS_PERCOLATION_H
#define BETWEENNESS_PERCOLATION_H
#include <percolation.h>
#include <vector>
#include <graph.h>
#include <heap_desc.h>
#include <betweenness.h>
#include <unionfind.h>
#include <thread>

class Percolation_betweenness : public Percolation {
private:
    int n_threads;
    void remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix);
    unsigned long int get_biggest(std::vector<std::vector<unsigned long int>> & adj_matrix);
public:
    Percolation_betweenness();
    Percolation_betweenness(int n_threads);
    void percolate(Graph &G);
    std::vector<double> get_result();
    std::vector<double> get_other_result();
    double get_mean_l();
};

#endif // BETWEENNESS_PERCOLATION_H

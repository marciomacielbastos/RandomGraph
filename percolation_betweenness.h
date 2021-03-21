#ifndef BETWEENNESS_PERCOLATION_H
#define BETWEENNESS_PERCOLATION_H
#include <percolation_vertex.h>
#include <vector>
#include <graph.h>
#include <heap_desc.h>
#include <betweenness.h>
#include <unionfind.h>
#include <thread>

class Percolation_betweenness : public Percolation_vertex {
private:
    int n_threads;
    void remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix);
    unsigned long int get_biggest(std::vector<std::vector<unsigned long int>> & adj_matrix);
public:
    Percolation_betweenness();
    Percolation_betweenness(int n_threads);
    void percolate(Graph &G);
    void percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks);
    std::vector<double> get_result();
};

#endif // BETWEENNESS_PERCOLATION_H

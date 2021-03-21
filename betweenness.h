#ifndef BETWEEENNESS_H
#define BETWEEENNESS_H
#include <vector>
#include <breadth_first_search.h>
#include <graph.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include <math.h>
#include <uniform.h>


class Betweenness {
    std::vector<double> betweenness;

    std::mutex mutex;

    double average_path;

    unsigned long int n_threads;

    void accumulation(std::vector<std::vector<unsigned long int>> &P,
                      std::vector<unsigned long int> &S,
                      std::vector<double> &sigma,  unsigned long int s);

    void rescale(unsigned long int k);

public:

    Betweenness();
    Betweenness(Graph & G, unsigned long int n_threads);
    Betweenness(std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int n_threads);

    void betweenness_centrality_given_adj_matrix_and_source(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int source);
    void betweenness_centrality_given_graph_and_source(Graph & G, unsigned long int source);
    void betweenness_centrality(Graph & G);
    void betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix);
    std::vector<double> get_betweenness_centrality_vector();
    double mean_l();
};

#endif // BETWEEENNESS_H

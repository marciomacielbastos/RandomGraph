#ifndef BETWEEENNESS_H
#define BETWEEENNESS_H
#include <vector>
#include <bfs.h>
#include <graph.h>
#include <thread>
#include <mutex>
#include <algorithm>
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
    Betweenness(std::vector<std::vector<unsigned long int>> &adj_matrix,
                unsigned long int n_threads);
    void betweenness_centrality_of_i(std::vector<std::vector<unsigned long int>> & adj_matrix,
                                     unsigned long int i);
    void betweenness_centrality_of_s(Graph & G, unsigned long int s);
    void betweenness_centrality(Graph & G);
    void betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix);
    std::vector<double> betweenness_centrality();

    double mean_l();
};

#endif // BETWEEENNESS_H

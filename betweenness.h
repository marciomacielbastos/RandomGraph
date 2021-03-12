#ifndef BETWEEENNESS_H
#define BETWEEENNESS_H
#include <vector>
#include <bfs.h>
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

    void accumulation_approx(unsigned long int d_size, std::vector<unsigned long int> &p, unsigned long int s, double r);

    void rescale(unsigned long int k);

    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);

    std::vector<unsigned long int> sample_pivots(unsigned long int N, unsigned long int sample_size);

    unsigned long int sample_parent(std::vector<std::vector<unsigned long int>>probabilities,
                                    std::vector<std::vector<unsigned long int>> P,
                                    unsigned long int w);

    std::vector<unsigned long int> build_path(const std::vector<std::vector<unsigned long int>> &probabilities,
                                              const std::vector<unsigned long> &S,
                                              const std::vector<std::vector<unsigned long> > &P);

    unsigned long int qsearch(unsigned long int idx, unsigned long int l_idx, unsigned long int u_idx, std::vector<unsigned long int> list);

public:

    Betweenness();

    Betweenness(Graph & G, unsigned long int n_threads);

    Betweenness(Graph &G, unsigned long int n_threads, unsigned long int sample_size, unsigned long int r);

    Betweenness(std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int n_threads);


    void betweenness_centrality_of_i(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int i);

    void betweenness_centrality_of_s(Graph & G, unsigned long int s);

    void betweenness_centrality_of_s_approx(Graph & G, unsigned long int N, unsigned long int s, unsigned long int r);

    void betweenness_centrality(Graph & G);

    void betweenness_centrality_approx(Graph & G, unsigned long int sample_size, unsigned long int r);

    void betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix);

    std::vector<double> betweenness_centrality();

    double mean_l();
};

#endif // BETWEEENNESS_H

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
private:
    Graph G;
    unsigned long int n_threads;
    std::vector<double> betweenness;
    std::mutex mutex;
    void create_betweenness_vector();
    void rescale(unsigned long int k);
    void flush_threads(std::vector<std::thread> &threads);
    void accumulation(std::vector<std::vector<unsigned long int>> &P,
                      std::vector<unsigned long int> &S,
                      std::vector<double> &sigma,
                      unsigned long int s);

public:
    Betweenness (Graph G);
    Betweenness(const Betweenness &b1);
    void set_num_of_threads(unsigned long int n);
    void betweenness_centrality_given_adj_matrix_and_source(std::vector<Rb_tree> & adj_matrix, unsigned long int source);
    void betweenness_centrality_from_source(unsigned long int source);
    void betweenness_centrality();
    void betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix);
    std::vector<double> get_betweenness_centrality_vector();
    void save (std::string filepath, std::string separator);
    void save (std::string filepath);
};


//class Betweenness {
//private:
//    Graph G;
//    std::vector<double> betweenness;
//    std::mutex mutex;
//    double average_path;
//    unsigned long int n_threads;
//    void accumulation(std::vector<std::vector<unsigned long int>> &P,
//                      std::vector<unsigned long int> &S,
//                      std::vector<double> &sigma,  unsigned long int s);
//    void rescale(unsigned long int k);

//public:
//    Betweenness(Graph G, unsigned long int n_threads);
//    Betweenness(std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int n_threads);

//    void betweenness_centrality_given_adj_matrix_and_source(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int source);
//    void betweenness_centrality_given_graph_and_source(unsigned long int source);
//    void betweenness_centrality();
//    void betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix);
//    std::vector<double> get_betweenness_centrality_vector();
//    double mean_l();
//    void save (std::string filepath, std::string separator);
//    void save (std::string filepath);
//};

#endif // BETWEEENNESS_H

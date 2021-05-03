#ifndef CLUSTERING_COEFFICIENT_H
#define CLUSTERING_COEFFICIENT_H
#include <vector>
#include <string>
#include <thread>
#include <unordered_map>

#include <graph.h>


class Clustering_coefficient {
private:
    Graph G;
    std::vector<std::vector<bool>> adjacent_matrix;
    std::vector<double> clustering_coefficient;
    double square_mean(const std::vector<unsigned long int> &degrees);
    double count_triangles(unsigned long int root);
    void clustering_of_vertex(unsigned long int i);
    void flush_threads(std::vector<std::thread> &threads);
    void create_clustering_coefficient_vector(unsigned long int N);
    void create_adjacent_matrix();
    void fill_adjacent_matrix();
    bool is_connected(unsigned long int v, unsigned long int w);

public:
    Clustering_coefficient(Graph G);
    std::vector<double> clustering();
    void clustering_multithread(unsigned long int n_threads);
    void build_adjacent_matrix();
    void save (std::string filepath);
    void save (std::string filepath, std::string separator);
};

#endif // CLUSTERING_COEFFICIENT_H

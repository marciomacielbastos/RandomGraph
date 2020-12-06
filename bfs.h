#ifndef BFS_H
#define BFS_H
#include <vector>
#include <queue>
#include <graph.h>
#include "uniform.h"

class Bfs
{
private:
    double average_path;
    std::vector<std::vector<unsigned long int>> P;
    std::vector<unsigned long int> S;
    std::vector<double> sigma;
    void start_vectors(unsigned long int N);

public:
    Bfs();
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    double bfs(unsigned long int root, std::vector<std::vector<unsigned long int>> adj_matrix);
    double bfs(std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int s);
    double bfs(Graph & g, unsigned long int s);
    void bfs(Graph & g, unsigned long int s, double & l);
    double avg_geo_dist(unsigned long int number_of_samples, std::vector<std::vector<unsigned long int>> adj_matrix);
    double get_l();
    std::vector<std::vector<unsigned long int>> getP();
    std::vector<unsigned long int> getS();
    std::vector<double> get_sigma();
};

#endif // BFS_H

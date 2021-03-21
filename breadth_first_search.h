#ifndef BFS_H
#define BFS_H
#include <vector>
#include <queue>
#include <graph.h>
#include "uniform.h"

class Breadth_first_search {
private:
    double average_geodesical_path;
    std::vector<std::vector<unsigned long int>> Parent_matrix;
    std::vector<unsigned long int> spanning_tree;
    std::vector<double> sigma;
    void build_algorithm_vectors(unsigned long int N);

public:
    Breadth_first_search();
    void search(Graph & G, unsigned long int source);
    void search(std::vector<std::vector<unsigned long int>> adj_matrix, unsigned long int source);
    double get_average_geodesical_path();
    std::vector<std::vector<unsigned long int>> get_parent_matrix();
    std::vector<unsigned long int> get_spanning_tree();
    std::vector<double> get_sigma();
};

#endif // BFS_H

#ifndef PERCOLATION_NODE_H
#define PERCOLATION_NODE_H
#include <uniform.h>
#include <unionfind.h>
#include <vector>
#include <graph.h>
#include <percolation.h>
#include <cmath>

class Percolation_vertex : public Percolation {
protected:
    std::vector<unsigned long int> build_identity_vector(unsigned long int N);
    std::vector<unsigned long int> define_ticks(double lower_bound, double upper_bound, unsigned long int number_of_ticks, unsigned long int N);   
    void smart_pop(std::vector<unsigned long> &vect, unsigned long int idx);
    void remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node);
    unsigned long int calculate_maximal_component_size(unsigned long int N, std::vector<std::vector<unsigned long int>> &adj_matrix);
public:
    Percolation_vertex();
    void percolate(Graph & G);
    void percolate_on_the_interval(Graph & G, double lower, double upper, unsigned long int number_of_ticks);
    std::vector<double> get_result();
    void save (std::string filepath);
    void save (std::string filepath, std::string separator);

};

#endif // PERCOLATION_NODE_H

#ifndef PERCOLATION_BIGGEST_DEGREE_H
#define PERCOLATION_BIGGEST_DEGREE_H
#include <heap_desc.h>
#include <unionfind.h>
#include <vector>
#include <graph.h>


class Percolation_degree {
private:
    std::vector<double> result;
public:
    Percolation_degree();
    void malicious_attack(Graph & G);
    std::vector<double> get_result();
    void smart_pop(std::vector<unsigned long int> & vect, unsigned long int idx);
    void remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node);
    void remove_node(std::vector<std::vector<unsigned long>> &adj_matrix, unsigned long int node);

};

#endif // PERCOLATION_BIGGEST_DEGREE_H

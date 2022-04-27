#ifndef PERCOLATION_BIGGEST_DEGREE_H
#define PERCOLATION_BIGGEST_DEGREE_H
#include <heap_desc.h>
#include <unionfind.h>
#include <vector>
#include <graph.h>
#include <percolation_random.h>
#include <cmath>


class Percolation_malicious : public Percolation_random {
protected:
    std::vector<unsigned long int> build_degree_vector(std::vector<std::vector<unsigned long int>> &adj_matrix);

public:
    Percolation_malicious();
    void percolate(Graph & G);
    void percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks);
    void percolate_molloy_reed(Graph & G);
    std::vector<double> get_result();
};

#endif // PERCOLATION_BIGGEST_DEGREE_H

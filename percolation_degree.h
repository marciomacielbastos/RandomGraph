#ifndef PERCOLATION_BIGGEST_DEGREE_H
#define PERCOLATION_BIGGEST_DEGREE_H
#include <heap_desc.h>
#include <unionfind.h>
#include <vector>
#include <graph.h>
#include <percolation.h>
#include <cmath>


class Percolation_degree : public Percolation {
private:
    double step;
    void smart_pop(std::vector<unsigned long int> & vect, unsigned long int idx);
    void remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node);
    void remove_node(std::vector<std::vector<unsigned long>> &adj_matrix, unsigned long int node);
    unsigned long int calc_max_comp(unsigned long int N, std::vector<std::vector<unsigned long int>> &adj_matrix);
    double stats_mean(std::vector<unsigned long int> vect);
    double stats_moment(std::vector<unsigned long int> vect, unsigned int moment, double mean);
    double binder_cumulant(std::vector<unsigned long int> vect);
    double binder_cumulant(std::vector<unsigned long> vect, double mean);
public:
    Percolation_degree();
    Percolation_degree(double step);
    void percolate(Graph & G);
    std::vector<double> get_result();
    std::vector<double> get_other_result();
    double stats_moment(std::vector<unsigned long int> vect, unsigned int moment);
};

#endif // PERCOLATION_BIGGEST_DEGREE_H

#ifndef PERCOLATION_H
#define PERCOLATION_H
#include <uniform.h>
#include <q_exponential.h>
#include <graph.h>

#include <vector>
#include <iostream>


class Percolation{
private:
    unsigned long int  N;
    unsigned long int lambda;
    unsigned long int gamma;
    unsigned long int xmin;
    Graph g;
    void progress_bar(double progress);
    std::vector<unsigned long> set_degree_lists(const std::deque<unsigned long> degree_list, std::deque<unsigned long>& l1, std::deque<unsigned long>& ln);
    void create_topology(std::vector<unsigned long> degree_list);

public:
    Percolation(unsigned long int  N, unsigned long int lambda, unsigned long int gamma, unsigned long int xmin);
    std::vector<unsigned long int> get_degree_list();
    double get_q(double gamma);
    Graph build_graph();
    std::vector<std::pair<unsigned long int, unsigned long int>> get_link_list();

};

#endif // PERCOLATION_H

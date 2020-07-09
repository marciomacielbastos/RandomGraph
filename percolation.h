#ifndef PERCOLATION_H
#define PERCOLATION_H
#include <uniform.h>
#include <distribution.h>
#include <q_exponential.h>
#include <graph.h>
#include <vector>
#include <iostream>
#include <unionfind.h>
#include <bfs.h>
#include <thread>
#include <topology_builder.h>
#include <functional>

class Percolation{
private:
    unsigned long int  N;
//    double lambda;
//    double q;
//    unsigned long int xmin;
    Distribution * probability_distribution;

    Graph g;
    void progress_bar(double progress);

public:
    Percolation(Distribution * probability_distribution, unsigned long int  N);
    std::vector<unsigned long int> get_degree_list();
    double get_q(double gamma);
    double geodesical_distance_computation(std::vector<std::vector<unsigned long int>> adj_matrix);
    double geodesical_distance_computation(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix);
    std::vector<std::vector<double>> percolation_molloy_reed_criterion(std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links,
                                                                       unsigned long int number_of_samples);
    std::vector<std::vector<double>> percolation_computation(unsigned int num_rep);

};

#endif // PERCOLATION_H

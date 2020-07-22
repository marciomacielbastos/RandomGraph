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
#include <fstream>
#include <sstream>


class Percolation{
private:
    unsigned long int  N;
    std::vector<std::vector<double>> molloy_reed_results;
    Distribution * probability_distribution;
    int thread_id;
    Graph g;
    void progress_bar(double progress, unsigned long int i, unsigned long int n);
    std::vector<unsigned long int> get_degree_list();
    double get_q(double gamma);
    double geodesical_distance_computation(std::vector<std::vector<unsigned long int>> adj_matrix);
    void t_geodesical_distance(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix);
    std::vector<std::vector<double>> percolation_molloy_reed_criterion(std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links,
                                                                       unsigned long int number_of_samples);
public:

    Percolation(Distribution * probability_distribution, unsigned long int  N);
    Percolation(Distribution * probability_distribution, unsigned long int  N, int thread_id);

    std::vector<std::vector<double>> percolation_molloy_reed(unsigned int num_rep);

    void write_percolation_results(const std::string& filename);




};

#endif // PERCOLATION_H

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
#include <heap_asc.h>
#include <betweenness.h>
#include <thread>
#include <topology_builder.h>
#include <topology_builder_configurational.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <percolation_betweenness.h>
#include <percolation_edge.h>
#include <percolation_kcore.h>
#include <percolation_degree.h>



class Percolation{
private:
    unsigned long int  N;
    unsigned long int noc;

    double mean_l;
    std::vector<std::vector<double>> pb_result;
    std::vector<std::vector<double>> pk_result;
    std::vector<std::vector<double>> pd_result;
    std::vector<std::vector<double>> pe_result;

    Distribution * probability_distribution;  
    void progress_bar(double progress, unsigned long int i, unsigned long int n);
    std::vector<unsigned long int> get_degree_list();
    std::vector<unsigned long int> get_modified_degree_list();
    double get_q(double gamma);

    void update_percolation_list(unsigned long n, unsigned long int j, std::vector<std::vector<double>> &results, std::vector<double> input);
    void update_percolation_list(unsigned long n, unsigned long int j, std::vector<std::vector<double>> &results, double input);
    void mean_std_percolation(unsigned long n, std::vector<std::vector<double>> &results, std::vector<std::vector<double>> input);
    void mean_std_percolation(unsigned long n, std::vector<std::vector<double>> &results, std::vector<double> input);
    void t_geodesical_distance(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix);
    void remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix);

public:

    Percolation();
    Percolation(Distribution * probability_distribution, unsigned long int  N, unsigned long noc);
    void percolation_configurational(unsigned int number_of_samples, unsigned long int n_threads);
    std::vector<std::vector<double>> get_betweenness_result();
    std::vector<std::vector<double>> get_kcore_result();
    std::vector<std::vector<double>> get_degree_result();
    std::vector<std::vector<double>> get_edge_result();
    double get_mean_l();
};

#endif // PERCOLATION_H

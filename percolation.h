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

class Percolation {
protected:
    std::vector<double> result;
    std::vector<double> other_result;

public:
   virtual std::vector<double> get_result() = 0;
   virtual std::vector<double> get_other_result() = 0;
   virtual void percolate(Graph &G) = 0;

//    unsigned long int  N;
//    unsigned long int noc;

//    double mean_l;
//    std::vector<std::vector<double>> pb_result;
//    std::vector<std::vector<double>> pk_result;
//    std::vector<std::vector<double>> pd_result;
//    std::vector<std::vector<double>> pe_result;

//    Distribution * probability_distribution;
//    void progress_bar(double progress, unsigned long int i, unsigned long int n);
//    std::vector<unsigned long int> get_degree_list();
//    std::vector<unsigned long int> get_modified_degree_list();
//    double get_q(double gamma);

//    void update_percolation_list(unsigned long n, unsigned long int j, std::vector<std::vector<double>> &results, std::vector<double> input);
//    void update_percolation_list(unsigned long n, unsigned long int j, std::vector<std::vector<double>> &results, double input);
//    void mean_std(unsigned long n, std::vector<std::vector<double>> &results, std::vector<std::vector<double>> input);
//    void mean_std(unsigned long int i, std::vector<std::vector<double>>& mean_results, std::vector<double> observed_values);
//    void t_geodesical_distance(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix);
//    void remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix);


//    Percolation(Distribution * probability_distribution, unsigned long int  N, unsigned long noc);
//    void percolation_statistics(unsigned int number_of_samples, unsigned long int n_threads);
//    std::vector<std::vector<double>> get_betweenness_result();
//    std::vector<std::vector<double>> get_kcore_result();
//    std::vector<std::vector<double>> get_degree_result();
//    std::vector<std::vector<double>> get_edge_result();
//    double get_mean_l();
};

#endif // PERCOLATION_H

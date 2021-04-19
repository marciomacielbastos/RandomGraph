#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H
#include <algorithm>
#include <string>
#include <thread>

#include <configurational_model.h>
#include <graph.h>
#include <power_law.h>
#include <q_exponential.h>


class q_Exp_graph_generator {
private:
    double gamma;
    double lambda;
    unsigned long int  kmin;
    unsigned long int  N;
    std::string filename_prefix;
    std::string graph_folder;
    std::string degrees_folder;

    std::vector<unsigned long int> sample_from_dist(Probability_distribution *dist);
    std::vector<unsigned long int> sample_degrees();
    void flush_threads(std::vector<std::thread> &threads);
    void graph_build_and_save_on_thread(std::vector<std::thread> &threads, unsigned int id);
    double calculate_tsallis_q();
    Graph calc_topology_with_configurational_method(std::vector<unsigned long int> degree_list);
    std::string make_filename_prefix();
    std::string make_filename_suffix(unsigned int id);
    std::string make_degree_filepath(unsigned int id);
    std::string make_graph_filepath(unsigned int id);

public:
    q_Exp_graph_generator(double gamma, double lambda, unsigned long int kmin, unsigned long N);
    void generate_multiple_graphs(unsigned long int repetitions, unsigned long int n_threads, unsigned long int from);
    void generate_multiple_graphs(unsigned long int repetitions, unsigned long int n_threads);
    Graph generate_graph(std::vector<unsigned long int> degrees_vector);
    void graph_build_and_save(unsigned int id);
    void write_degrees (Graph G, std::string filepath);
    void set_degrees_folder(std::string folder);
    void set_graph_folder(std::string folder);




};

#endif // GRAPH_GENERATOR_H

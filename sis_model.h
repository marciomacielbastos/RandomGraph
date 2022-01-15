#ifndef SIS_MODEL_H
#define SIS_MODEL_H

#include <vector>
#include <list>
#include <string>

#include <graph.h>
#include <uniform.h>
#include <poisson.h>

class Sis_model {
private:
    double probability_to_infect;
    double lambda;
    std::list<unsigned long int> infectious;
    std::vector<unsigned long int> infectious_time;
    std::vector<unsigned long int> infectious_record;
    Graph g;

    void cure();
    bool try_cure(unsigned long int node_id);
    std::list<unsigned long int> infect();
    bool try_infect(unsigned long int node_id);
    void update_infectious(std::list<unsigned long int> infecteds);
    void update_expected_infection_rate();
    void update_expected_cure_rate();

public:
    Sis_model();
    void set_graph(Graph g);
    void set_probability_to_infect(double p);
    void set_probability_to_cure(double p);
    void set_mean_infectious_interval(double t);
    bool infectious_is_empty();
    void run_dynamics();
    void randomly_infect();
    void record_number_of_infecteds();
    std::vector<unsigned long> get_infectious_record();
    void save(std::string filepath, std::string separator);
    void restart();
};

#endif // SIS_MODEL_H

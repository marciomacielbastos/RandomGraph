#ifndef TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#define TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#include <vector>
#include <graph.h>

class Configurational_model {
private:
    bool built;
    unsigned long int number_available;
    std::vector<unsigned long int> degrees_vector;
    Graph g;
    bool is_connected(unsigned long v, unsigned long w);
    bool link(std::vector<unsigned long int> &list, unsigned long v_idx, unsigned long w_idx);
    std::vector< unsigned long int> mount_algorithmic_vector();
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    std::vector<std::pair<unsigned long int, unsigned long int>> shuffle(std::vector<std::pair<unsigned long int, unsigned long int>> &algorithmic_vector); 
    unsigned long int sample_from_dist(std::vector<unsigned long int> &stubs_dist);
    void collect_stubs(std::vector<unsigned long int> &algorithmic_vector, unsigned long v_idx, int &counter);
    void collect_stubs(unsigned long v, int &counter);
    void update_algorithmic_vector(std::vector<unsigned long int> &vector,
                                   unsigned long int v_idx,
                                   unsigned long int w_idx);

public:
    Configurational_model(std::vector<unsigned long int> dl);
    bool is_built();
    bool random_link(bool samp);
    bool random_link();
    Graph get_g(); 
};

#endif // TOPOLOGY_BUILDER_CONFIGURATIONAL_H

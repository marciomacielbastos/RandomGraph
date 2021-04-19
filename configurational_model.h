#ifndef TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#define TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#include <vector>
#include <graph.h>

class Configurational_model {
private:
    bool built;
    std::vector<unsigned long int> degrees_vector;
    Graph g;
    bool is_connected(unsigned long v, unsigned long w);
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    void update_algorithmic_vector(std::vector<unsigned long int> &list, unsigned long int v_idx, unsigned long int w_idx);
    bool link(std::vector<unsigned long int> &list, unsigned long v, unsigned long w);
    std::vector<unsigned long int> mount_algorithmic_vector();
    void try_hard(std::vector<unsigned long int> &algorithm_list, unsigned long v_idx, int &counter);

public:
    Configurational_model(std::vector<unsigned long int> dl);
    bool random_link();
    Graph get_g();
    bool is_built();
};

#endif // TOPOLOGY_BUILDER_CONFIGURATIONAL_H

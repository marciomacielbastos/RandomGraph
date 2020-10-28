#ifndef TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#define TOPOLOGY_BUILDER_CONFIGURATIONAL_H
#include <vector>
#include <graph.h>

class TopologyBuilderConfigurational {
private:
    bool built;
    std::vector<unsigned long int> degree_list;
    Graph g;
    bool is_connected(unsigned long v, unsigned long w);
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    void update_algolist(std::vector<unsigned long int> &list, unsigned long int v_idx, unsigned long int w_idx);
    bool link(std::vector<unsigned long int> &list, unsigned long v, unsigned long w);
    std::vector<unsigned long int> mount_algorithm_list();

public:
    TopologyBuilderConfigurational(std::vector<unsigned long int> dl);
    bool random_link();
    Graph get_g();
    bool is_built();
};

#endif // TOPOLOGY_BUILDER_CONFIGURATIONAL_H

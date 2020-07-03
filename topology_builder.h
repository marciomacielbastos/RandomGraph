#ifndef TOPOLOGY_BUILDER_H
#define TOPOLOGY_BUILDER_H
#include <vector>
#include <set>
#include <graph.h>
#include <iostream>
#include <algorithm>

class Topology_builder{
private:
    unsigned long int degree_1_counter;
    unsigned long int n_of_loose_ends;
    std::vector<unsigned long int> degree_list;
    unsigned long int k_clique;
    std::vector<unsigned long int> unbonded_nodes_l;
    std::vector<unsigned long int> unbonded_nodes_g;
    std::vector<unsigned long int> bonded_nodes;
    Graph g;

    void progress_bar(double progress);

    void pre_process();
    std::vector<unsigned long int> create_unbonded_nodes(unsigned long int N);
    void break_unbonded_nodes(std::vector<unsigned long int> &unbonded_nodes);
    void speckle_overbond();
    unsigned long int one_deg_pop();
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    bool link(unsigned long int v, unsigned long int w);
    void update_degree(unsigned long int v);


    void agglutination();
    void agglutination_underclique();
    unsigned long int un_pop(unsigned long int idx);
    void bn_push(unsigned long int w);
    void update_bn(unsigned long int v_idx);
    void update_bn(unsigned long int v_idx, unsigned long int w_idx);
    void speckle();
    void agglutination_overclique();
    void remove_duplicates(std::vector<unsigned long int> & vec);

    void other_connections();
    void bn_pop(unsigned long int idx);   


public:
    Topology_builder(std::vector<unsigned long int> dl);
    Graph get_g();

};

#endif // TOPOLOGY_BUILDER_H

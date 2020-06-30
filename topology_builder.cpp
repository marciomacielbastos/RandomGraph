#include "topology_builder.h"


/*
 * Factory Constructor of the Graph Topology
 * In this constructor we receive a list of nodes' degree, and build the topology through 3 stages.
 * First, pre-process. Dividing the nodes with degree one, degree below and degree above the equivalent clique mean degree (ECMD),
 * Then we spread the one degree nodes (ODN) through the nodes above the ECMD, until no node stays above the ECMD or none ODN remains.
 * Second, agglutination. In this method, we randomly create a path between each node in the graph;
 * Finally we randomly complete the missing links leaving the minimum number of loose ends.
*/
Topology_builder::Topology_builder(std::vector<unsigned long int> dl){
    this->degree_1_counter = 0;
    this->degree_list = dl;
    this->g = Graph(dl.size());
    std::cout << "pre-process()" << std::endl;
    pre_process();
    std::cout << "agglutination()" << std::endl;
    agglutination();
    std::cout << "other_connections()" << std::endl;
    other_connections();
    std::cout << "finish" << std::endl;
}

void Topology_builder::progress_bar(double progress){
    unsigned int bar_width = 70;
    std::cout << "[";
    unsigned int pos = static_cast<unsigned int>(double(bar_width) * progress);
    for (unsigned int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<unsigned int>(progress * 100.0) << " %  \r";
    std::cout.flush();
    if(progress == 1){
        std::cout << std::endl;
    }
}

std::vector<unsigned long int> Topology_builder::create_unbonded_nodes(unsigned long int N) {
    std::vector<unsigned long int> unbonded_nodes;
    for(unsigned long int i = 0; i < N; i++){
        if(degree_list[i] == 1){
            this->degree_1_counter++;
        }
        else{
            unbonded_nodes.push_back(i);
        }
    }
    this->k_clique = unbonded_nodes.size() - 1;
    return unbonded_nodes;
}

void Topology_builder::break_unbonded_nodes(std::vector<unsigned long int> &unbonded_nodes) {
    while(!unbonded_nodes.empty()){
        unsigned long int idx = unbonded_nodes.back();
        unbonded_nodes.pop_back();
        if(this->degree_list[idx] > this->k_clique){
            this->unbonded_nodes_g.push_back(idx);
        }
        else {
            this->unbonded_nodes_l.push_back(idx);
        }
    }
}

unsigned long int Topology_builder::one_deg_pop() {
    unsigned long int v = this->degree_1_counter - 1;
    this->degree_1_counter--;
    return v;
}

void Topology_builder::update_degree(unsigned long int v) {
    this->degree_list[v]--;
}

bool Topology_builder::link(unsigned long v, unsigned long w) {
    if (g.link(v, w)) {
        update_degree(v);
        update_degree(w);
        return true;
    }
    else {
        return false;
    }
}

void Topology_builder::speckle_overbond() {
    unsigned long int v, w, idx;
    Uniform u;
    while ((this->degree_1_counter > 0) && (!this->unbonded_nodes_g.empty())) {
        idx = u.randint(this->unbonded_nodes_g.size());
        v = one_deg_pop();
        w = this->unbonded_nodes_g[idx];
        link(v, w);
        if (this->degree_list[w] <= this->k_clique) {
            this->unbonded_nodes_l.push_back(w);
            smart_pop(this->unbonded_nodes_g, idx);
        }
    }
}

void Topology_builder::pre_process() {
    unsigned long int N = this->degree_list.size();
    std::sort (this->degree_list.begin(), this->degree_list.end());
    std::vector<unsigned long int> unbonded_nodes = create_unbonded_nodes(N);
    break_unbonded_nodes(unbonded_nodes);
    speckle_overbond();
}

unsigned long int Topology_builder::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

unsigned long int Topology_builder::un_pop(unsigned long int w_idx) {
    unsigned long int un_l_size = this->unbonded_nodes_l.size();
    if (w_idx < un_l_size) {
        return smart_pop(this->unbonded_nodes_l, w_idx);
    }
    else {
        w_idx = w_idx - un_l_size;
        return smart_pop(this->unbonded_nodes_g, w_idx);
    }
}

void Topology_builder::bn_push(unsigned long int w) {
    this->bonded_nodes.push_back(w);
}

void Topology_builder::update_bn(unsigned long int v_idx) {
    unsigned long int deg = this->degree_list[this->bonded_nodes[v_idx]];
    if(deg == 0) smart_pop(this->bonded_nodes, v_idx);
}

void Topology_builder::update_bn(unsigned long int v_idx, unsigned long int w_idx) {
    if (v_idx < w_idx)  {
        update_bn(w_idx);
        update_bn(v_idx);
    }
    else if (w_idx < v_idx) {
        update_bn(v_idx);
        update_bn(w_idx);
    }
}

void Topology_builder::agglutination_underclique() {
    Uniform u;
    unsigned long int w, v, w_idx, v_idx;
    unsigned long int bn_size = this->bonded_nodes.size();
    unsigned long int un_size = this->unbonded_nodes_l.size();
    while (un_size > 0) {
        // No bonded nodes
        if (bn_size == 0) {
            w_idx = u.randint(un_size);
            w = un_pop(w_idx);
            bn_push(w);
            bn_size = this->bonded_nodes.size();
        }

        else {
           v_idx = u.randint(bn_size);
           v = this->bonded_nodes[v_idx];
           w_idx = u.randint(un_size);
           w = un_pop(w_idx);
           link(v, w);
           bn_push(w);
           bn_size = this->bonded_nodes.size();
           update_bn(v_idx);
        }
        un_size--;
    }
}

void Topology_builder::speckle(){
    unsigned long int v, w, w_idx;
    unsigned long int bn_size = this->bonded_nodes.size();
    Uniform u;
    while (this->degree_1_counter > 0) {
        v = one_deg_pop();
        w_idx = u.randint(bn_size);
        w = this->bonded_nodes[w_idx];
        link(v, w);
        update_bn(w_idx);
        bn_size = this->bonded_nodes.size();
    }
}

void Topology_builder::agglutination_overclique() {
    Uniform u;
    unsigned long int w, v, w_idx, v_idx;
    unsigned long int bn_size = this->bonded_nodes.size();
    unsigned long int un_l_size = this->unbonded_nodes_l.size();
    unsigned long int un_g_size = this->unbonded_nodes_g.size();
    unsigned long int un_size = un_l_size + un_g_size;
    unsigned long int num_overclique = un_g_size;
    // As long as there are nodes with degree above ECMD or remains nodes in LU do:
    while ((num_overclique > 0) && (un_size > 0)) {
        // If no node was agglutinated, randomly pop from the list of unbonded (LU) and push in the list of bonded nodes (LB).
        if (bn_size == 0) {
            w_idx = u.randint(un_size);
            w = un_pop(w_idx);
            bn_push(w);
            bn_size = this->bonded_nodes.size();
        }
        //If the are nodes in the LB, randomly choose one from LB, and link to another in LU, according to the procedure:
        else {
           v_idx = u.randint(bn_size);
           v = this->bonded_nodes[v_idx];
           // If the degree of the node poped from LB is bellow or equal ECMD, then select a random node v from the {v in LU | deg(v) above ECMD};
           if (this->degree_list[v] <= this->k_clique) {
               w_idx = u.randint(un_l_size, un_size);
               w = un_pop(w_idx);
           }
           // Else any link is allowed, thus select any node from LU
           else {
               w_idx = u.randint(un_size);
               w = un_pop(w_idx);
           }
           //If the nodes' degree are no longer above ECMD, then reduce num_overclique.
           if((this->degree_list[v] > this->k_clique) && (this->degree_list[v] - 1 <= this->k_clique)) num_overclique--;
           if((this->degree_list[w] > this->k_clique) && (this->degree_list[w] - 1 <= this->k_clique)) num_overclique--;
           link(v, w);
           bn_push(w);
           bn_size = this->bonded_nodes.size();
           update_bn(v_idx, bn_size - 1);
        }
        //Update the size of the LU lists.
        un_l_size = this->unbonded_nodes_l.size();
        un_g_size = this->unbonded_nodes_g.size();
        un_size = un_l_size + un_g_size;
    }
}

void Topology_builder::agglutination(){
    //There ain't no overbond nodes
    if(this->unbonded_nodes_g.size() == 0){
        agglutination_underclique();
        //Speckle ODN through the just linked graph
        speckle();
    }
    else {
        agglutination_overclique();
        agglutination_underclique();
    }
}


void Topology_builder::other_connections(){
    Uniform u;
    unsigned long int idx_v, idx_w, v, w, size;
    size = this->bonded_nodes.size();
    while (this->bonded_nodes.size() > 1) {
        unsigned long int i = 0;        
        idx_v = u.randint(size);
        v = this->bonded_nodes[idx_v];
        if (this->degree_list[v] == 0) {
            smart_pop(this->bonded_nodes, idx_v);
            continue;
        }
        std::vector<bool> valuation(size);
        while((this->degree_list[v] > 0) && i < size){
            idx_w = u.randint(size);
            w = this->bonded_nodes[idx_w];
            if(!valuation[idx_w]){
                std::vector<bool>::reference ref = valuation[idx_w];
                ref = true;
                i++;
            }
            link(v, w);
        }
        smart_pop(this->bonded_nodes, idx_v);
        size = this->bonded_nodes.size();
    }
}

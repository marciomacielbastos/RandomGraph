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
    auto start = std::chrono::high_resolution_clock::now();
    pre_process();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "pre_process duration: (" << duration.count() << " millisecond)"<< std::endl;
    agglutination();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "agglutination duration: (" << duration.count() << " millisecond)"<< std::endl;
    other_connections();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "other connections duration: (" << duration.count() << " millisecond)"<< std::endl;
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
    if (g.link(v, w) && (this->degree_list[v] && this->degree_list[w])) {
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
    while ((this->degree_1_counter > 0) && bn_size > 0) {
        v = one_deg_pop();
        w_idx = u.randint(bn_size);
        w = this->bonded_nodes[w_idx];
        link(v, w);
        update_bn(w_idx);
        bn_size = this->bonded_nodes.size();
    }
    unsigned long int i_min = this->degree_1_counter;
    if (i_min > 0) {
        while(this->degree_1_counter > 0){
            w = u.randint(i_min, this->degree_list.size());
            v = one_deg_pop();
            this->degree_list[w]++;
            link(v, w);
        }
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
           if ((this->degree_list[v] <= this->k_clique) && (un_g_size > 0)) {
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
        //Speckle ODN through tree like graph
        speckle();
    }
    else {
        agglutination_overclique();
        agglutination_underclique();
    }
}


void Topology_builder::other_connections(){
    Uniform u;
    unsigned long int number_of_nodes_to_link = 0;
    unsigned long int idx_v, idx_w, v, w, id_min, id_max;
    if(!this->bonded_nodes.empty())  number_of_nodes_to_link = this->bonded_nodes.size();
    std::vector<unsigned long int> zeroes;
    while (number_of_nodes_to_link > 1) {
        unsigned long int i = 0;
        id_min = 0;
        idx_v = u.randint(number_of_nodes_to_link);
        v = this->bonded_nodes[idx_v];
        smart_pop(this->bonded_nodes, idx_v);
        number_of_nodes_to_link = this->bonded_nodes.size();
        id_max = number_of_nodes_to_link;
        std::vector<bool> linked_map(number_of_nodes_to_link);
        while((this->degree_list[v] > 0) && i < number_of_nodes_to_link){
            idx_w = u.randint(id_min, id_max);
            if (idx_w == id_min) id_min++;
            if (idx_w == id_max) id_max--;
            w = this->bonded_nodes[idx_w];
            if(!linked_map[idx_w]){
                std::vector<bool>::reference ref = linked_map[idx_w];
                ref = true;
                i++;
            }
            link(v, w);
            if(this->degree_list[w] == 0){
                zeroes.push_back(idx_w);
            }
        }
        while (!zeroes.empty()) {
            unsigned long int id = zeroes.back();
            zeroes.pop_back();
            smart_pop(this->bonded_nodes, id);
        }
        number_of_nodes_to_link = this->bonded_nodes.size();
    }
}

Graph Topology_builder::get_g(){
    return this->g;
}

#include "topology_builder_configurational.h"

TopologyBuilderConfigurational::TopologyBuilderConfigurational(std::vector<unsigned long int> dl) {
    this->degree_list = dl;
    this->g = Graph(dl.size());
    this->built = false;
}

bool TopologyBuilderConfigurational::is_built(){
    return this->built;
}

bool TopologyBuilderConfigurational::is_connected(unsigned long v, unsigned long w) {
    if (this->g.is_connected(v,w)) {
        return true;
    } else {
        return false;
    }
}



std::vector<unsigned long int> TopologyBuilderConfigurational::mount_algorithm_list(){
    std::vector<unsigned long int> algorithm_list;
    for(unsigned long int i = 0; i < this->degree_list.size(); i++){
        for(unsigned long int j = 0; j < this->degree_list[i]; j++){
            algorithm_list.push_back(i);
        }
    }
    return algorithm_list;
}

unsigned long int TopologyBuilderConfigurational::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

void TopologyBuilderConfigurational::update_algolist(std::vector<unsigned long int> &list, unsigned long int v_idx, unsigned long int w_idx) {
    if (v_idx < w_idx)  {
        smart_pop(list, w_idx);
        smart_pop(list, v_idx);
    }
    else if (w_idx < v_idx) {
        smart_pop(list, v_idx);
        smart_pop(list, w_idx);
    }
}

bool TopologyBuilderConfigurational::link(std::vector<unsigned long int> &list, unsigned long v, unsigned long w) {
    if (this->g.link(v, w)) {
        update_algolist(list, v, w);
        return true;
    }
    else {
        return false;
    }
}

bool TopologyBuilderConfigurational::random_link() {
    std::vector<unsigned long int> algorithm_list = mount_algorithm_list();
    Uniform u;  
    unsigned long int v, w, v_idx, w_idx;
    int counter;
    while (algorithm_list.size() > 1) {
        v_idx = u.randint(algorithm_list.size());
        w_idx = u.randint(algorithm_list.size());

        v = algorithm_list[v_idx];
        w = algorithm_list[w_idx];
        counter = 0;
        while((v == w) || (is_connected(v, w))){
            if(counter >= 100){
                return false;
            }
            v_idx = u.randint(algorithm_list.size());
            w_idx = u.randint(algorithm_list.size());

            v = algorithm_list[v_idx];
            w = algorithm_list[w_idx];
            counter++;
        }
        link(algorithm_list, v, w);
    }
    this->built = true;
    return true;
}

Graph TopologyBuilderConfigurational::get_g(){
    return this->g;
}

#include "configurational_model.h"

Configurational_model::Configurational_model(std::vector<unsigned long int> dl) {
    this->degrees_vector = dl;
    this->g = Graph(dl.size());
    this->built = false;
}

bool Configurational_model::is_built(){
    return this->built;
}

bool Configurational_model::is_connected(unsigned long v, unsigned long w) {
    if (this->g.is_connected(v,w)) {
        return true;
    } else {
        return false;
    }
}

std::vector<std::pair<unsigned long int, unsigned long int>> Configurational_model::shuffle(std::vector<std::pair<unsigned long int, unsigned long int>> &algorithmic_vector) {
    std::vector<std::pair<unsigned long int, unsigned long int>> shuffled_vector;
    Uniform u;
    unsigned long int idx;
    while (!algorithmic_vector.empty()) {
        idx = u.randint(algorithmic_vector.size());
        shuffled_vector.push_back(algorithmic_vector[idx]);
        algorithmic_vector[idx] = algorithmic_vector[algorithmic_vector.size() - 1];
        algorithmic_vector.pop_back();
    }
    return shuffled_vector;
}



std::vector< unsigned long int> Configurational_model::mount_algorithmic_vector(){
    std::vector<unsigned long int> algorithmic_vector;
    for(unsigned long int i=0; i < this->degrees_vector.size(); i++){
        for(unsigned long int j=0; j < this->degrees_vector[i]; j++){
            algorithmic_vector.push_back(i);
        }
    }
    return algorithmic_vector;
}

unsigned long int Configurational_model::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    if (!list.empty()) {
        list.pop_back();
    }
    return temp;
}

void Configurational_model::update_algorithmic_vector(std::vector<unsigned long int> &vector, unsigned long int v_idx, unsigned long int w_idx) {
    if (w_idx < v_idx) {
        unsigned long int temp = w_idx;
        w_idx = v_idx;
        v_idx = temp;
    }
    smart_pop(vector, w_idx);
    smart_pop(vector, v_idx);
}

bool Configurational_model::link(std::vector<unsigned long int> &list, unsigned long v_idx, unsigned long w_idx) {
    unsigned long int v = list[v_idx];
    unsigned long int w = list[w_idx];
    if (this->g.link(v, w)) {
        update_algorithmic_vector(list, v_idx, w_idx);
        return true;
    }
    else {
        return false;
    }
}

void Configurational_model::collect_stubs(std::vector<unsigned long int> &algorithmic_vector, unsigned long v_idx, int &counter) {
    unsigned long int v = algorithmic_vector[v_idx];
    unsigned long int w;
    unsigned long int size = algorithmic_vector.size();
    std::vector<unsigned long int> popper;
    for (unsigned long int w_idx=0; w_idx < size; w_idx++) {
        w = algorithmic_vector[w_idx];
        if (!is_connected(v, w)) {
            link(algorithmic_vector, v, w);
            counter = 0;
            return;
        }
        else if (v == algorithmic_vector[w_idx]) {
            popper.push_back(w_idx);
        }
    }
    if (counter > 0) {
        while (!popper.empty()) {
            v_idx = popper.back();
            popper.pop_back();
            smart_pop(algorithmic_vector, v_idx);
        }
    }
    return;
}

bool Configurational_model::random_link() {
    Uniform u;
    std::vector<unsigned long int> algorithmic_vector = mount_algorithmic_vector();
    unsigned long int v, w, v_idx, w_idx;
    int counter;
    while (algorithmic_vector.size() > 1) {
        v_idx = u.randint(algorithmic_vector.size());
        w_idx = u.randint(algorithmic_vector.size());
        v = algorithmic_vector[v_idx];
        w = algorithmic_vector[w_idx];
        counter = 0;
        while(is_connected(v, w)){
            if (counter >= 100){
                collect_stubs(algorithmic_vector, v_idx, counter);
            }
            if (algorithmic_vector.size() <= 1) break;
            v_idx = u.randint(algorithmic_vector.size());
            w_idx = u.randint(algorithmic_vector.size());
            v = algorithmic_vector[v_idx];
            w = algorithmic_vector[w_idx];
            counter++;
        }
        if (!is_connected(v, w)) {
            link(algorithmic_vector,v_idx,w_idx);
        }
    }
    this->built = true;
    return true;
}

Graph Configurational_model::get_g(){
    return this->g;
}

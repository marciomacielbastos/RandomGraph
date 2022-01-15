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

std::vector<unsigned long int> Configurational_model::shuffle(std::vector<unsigned long int> &algorithmic_vector) {
    std::vector<unsigned long int> shuffled_vector;
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



std::vector<unsigned long int> Configurational_model::mount_algorithmic_vector(){
    std::vector<unsigned long int> algorithmic_vector;
    for(unsigned long int i = 0; i < this->degrees_vector.size(); i++){
        for(unsigned long int j = 0; j < this->degrees_vector[i]; j++){
            algorithmic_vector.push_back(i);
        }
    }
    algorithmic_vector = this->shuffle(algorithmic_vector);
    return algorithmic_vector;
}

unsigned long int Configurational_model::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

void Configurational_model::update_algorithmic_vector(std::vector<unsigned long int> &vector, unsigned long int v_idx, unsigned long int w_idx) {
    if (v_idx < w_idx)  {
        smart_pop(vector, w_idx);
        smart_pop(vector, v_idx);
    }
    else if (w_idx < v_idx) {
        smart_pop(vector, v_idx);
        smart_pop(vector, w_idx);
    }
}

bool Configurational_model::link(std::vector<unsigned long int> &list, unsigned long v_idx, unsigned long w_idx) {
    if (this->g.link(list[v_idx], list[w_idx])) {
        update_algorithmic_vector(list, v_idx, w_idx);
        return true;
    }
    else {
        return false;
    }
}

void Configurational_model::try_hard(std::vector<unsigned long int> &algorithmic_vector, unsigned long v_idx, int &counter) {
    unsigned long int v = algorithmic_vector[v_idx];
    unsigned long int w;
    unsigned long int size = algorithmic_vector.size();
    for (unsigned long int w_idx=0; w_idx < size; w_idx++) {
        w = algorithmic_vector[w_idx];
        if (!is_connected(v, w)) {
            link(algorithmic_vector, v_idx, w_idx);
            counter = 0;
        }
    }
    if (counter > 0) {
        smart_pop(algorithmic_vector, v_idx);
        counter = 0;
    }
}

bool Configurational_model::random_link() {
    std::vector<unsigned long int> algorithmic_vector = mount_algorithmic_vector();
    Uniform u;  
    unsigned long int v, w, v_idx, w_idx;
    int counter;
    while (algorithmic_vector.size() > 1) {
        v_idx = u.randint(algorithmic_vector.size());
        w_idx = u.randint(algorithmic_vector.size());
        v = algorithmic_vector[v_idx];
        w = algorithmic_vector[w_idx];
        counter = 0;
        while(is_connected(v, w)){
            if(counter >= 100){
                try_hard(algorithmic_vector, v_idx, counter);
                break;
            }
            if (algorithmic_vector.size() < 2) {
                return true;
            }
            v_idx = u.randint(algorithmic_vector.size());
            w_idx = u.randint(algorithmic_vector.size());

            v = algorithmic_vector[v_idx];
            w = algorithmic_vector[w_idx];
            counter++;
        }
        if (!is_connected(v, w)) link(algorithmic_vector, v_idx, w_idx);
    }
    this->built = true;
    return true;
}

Graph Configurational_model::get_g(){
    return this->g;
}

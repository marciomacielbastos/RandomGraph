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



std::vector<std::pair<unsigned long int, unsigned long int>> Configurational_model::mount_algorithmic_vector(){
    std::vector<std::pair<unsigned long int, unsigned long int>> algorithmic_vector;
    for(unsigned long int i = 0; i < this->degrees_vector.size(); i++){
        std::pair<unsigned long int, unsigned long int> p (i, this->degrees_vector[i]);
        algorithmic_vector.push_back(p);
    }
    return algorithmic_vector;
}

std::pair<unsigned long int, unsigned long int> Configurational_model::smart_pop(std::vector<std::pair<unsigned long int, unsigned long int>> &list, unsigned long int idx) {
    std::pair<unsigned long int, unsigned long int> temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

void Configurational_model::update_algorithmic_vector(std::vector<std::pair<unsigned long int, unsigned long int>> &vector, unsigned long int v_idx, unsigned long int w_idx) {
    if (w_idx < v_idx) {
        unsigned long int temp = w_idx;
        w_idx = v_idx;
        v_idx = temp;
    }
    vector[w_idx].second--;
    if (vector[w_idx].second < 1) {
        smart_pop(vector, w_idx);
    }
    vector[v_idx].second--;
    if (vector[v_idx].second < 1) {
        smart_pop(vector, v_idx);
    }
}

bool Configurational_model::link(std::vector<std::pair<unsigned long int, unsigned long int>> &list, unsigned long v_idx, unsigned long w_idx) {
    if (this->g.link(list[v_idx].first, list[w_idx].first)) {
        update_algorithmic_vector(list, v_idx, w_idx);
        return true;
    }
    else {
        return false;
    }
}

void Configurational_model::try_hard(std::vector<std::pair<unsigned long int, unsigned long int>> &algorithmic_vector, unsigned long v_idx, int &counter) {
    unsigned long int v = algorithmic_vector[v_idx].first;
    unsigned long int w;
    unsigned long int size = algorithmic_vector.size();
    for (unsigned long int w_idx=0; w_idx < size; w_idx++) {
        w = algorithmic_vector[w_idx].first;
        if (!is_connected(v, w)) {
            link(algorithmic_vector, v_idx, w_idx);
            counter = 0;
        }
    }
    shuffle((algorithmic_vector));
    if (counter > 0) {
        smart_pop(algorithmic_vector, v_idx);
        counter = 0;
    }
}

bool Configurational_model::random_link() {
    std::vector<std::pair<unsigned long int, unsigned long int>> algorithmic_vector = mount_algorithmic_vector();
    Uniform u;  
    unsigned long int v, w, v_idx, w_idx;
    int counter;
    bool tried_hard = false;
    while (algorithmic_vector.size() > 1) {
        v_idx = u.randint(algorithmic_vector.size());
        w_idx = u.randint(algorithmic_vector.size());
        v = algorithmic_vector[v_idx].first;
        w = algorithmic_vector[w_idx].first;
        counter = 0;
        while(is_connected(v, w)){
            if(counter >= 100){
                try_hard(algorithmic_vector, v_idx, counter);
                tried_hard = true;
                break;
            }
            if (algorithmic_vector.size() < 2) {
                return true;
            }
            v_idx = u.randint(algorithmic_vector.size());
            w_idx = u.randint(algorithmic_vector.size());

            v = algorithmic_vector[v_idx].first;
            w = algorithmic_vector[w_idx].first;
            counter++;
        }
        if (tried_hard) tried_hard = false;
        else {
            link(algorithmic_vector, v_idx, w_idx);
        }
    }
    this->built = true;
    return true;
}

Graph Configurational_model::get_g(){
    return this->g;
}

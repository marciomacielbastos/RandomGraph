#include "graph.h"

Graph::Graph(){

}

Graph::Graph(unsigned long int N){
    for(unsigned long int i = 0; i < N; i++){
        std::vector<unsigned long int> v;
        this->adj_matrix.push_back(v);
    }
}

//Graph::Graph(Graph g){
//    this->link_list = std::vector<std::pair<unsigned long int, unsigned long int>>(g.get_link_list());
//}

bool Graph::is_connected(unsigned long v, unsigned long w){
    if(v == w){
        return true;
    }
    if(this->adj_matrix[v].size() <= this->adj_matrix[w].size()){
        for(auto x : this->adj_matrix[v]){
            if(x == w){
                return true;
            }
        }
    } else {
        for(auto x : this->adj_matrix[w]){
            if(x == v){
                return true;
            }
        }
    }
    return false;
}

bool Graph::link(unsigned long v, unsigned long w){
    if(!is_connected(v, w)){
        std::pair<unsigned long int, unsigned long int> edge(v, w);
        this->link_list.push_back(edge);
        this->adj_matrix[v].push_back(w);
        this->adj_matrix[w].push_back(v);
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::pair<unsigned long int, unsigned long int>> Graph::get_link_list(){
    return this->link_list;
}

std::vector<unsigned long int> Graph::get_real_dist(){
    std::vector<unsigned long int> dist;
    for (unsigned long int i=0; i < this->adj_matrix.size(); i++) {
        dist.push_back(this->adj_matrix[i].size());
    }
    return dist;
}

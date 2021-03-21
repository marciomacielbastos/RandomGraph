#include "graph.h"

Graph::Graph(){

}

Graph::Graph(unsigned long int N){
    for(unsigned long int i = 0; i < N; i++){
        std::vector<unsigned long int> v;
        this->adj_matrix.push_back(v);
    }
    this->N = N;
}

void Graph::read_file(std::string filename, char delimiter) {
    std::string line;
    std::ifstream myfile (filename);
    std::string item;
    unsigned long int edge[2];
    if (myfile.is_open()) {
      while ( std::getline (myfile,line) ) {
        std::istringstream iss(line);
        int i = 0;
        while (std::getline(iss, item, delimiter)) {
              edge[i] = std::stoul(item);
              i++;
        }
        link(edge[0], edge[1]);
      }
      myfile.close();
    }

    else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

bool Graph::is_connected(unsigned long v, unsigned long w) {
    if(v == w){
        return true;
    }
    if (this->adj_matrix[v].size() <= this->adj_matrix[w].size()) {
        for (auto x : this->adj_matrix[v]) {
            if (x == w) {
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

bool Graph::link(unsigned long v, unsigned long w) {
    if(v > this->N || w> this->N) {
        int i =0;
    }
    if(!is_connected(v, w)){
        std::pair<unsigned long int, unsigned long int> edge(v, w);
        this->links_vector.push_back(edge);
        this->adj_matrix[v].push_back(w);
        this->adj_matrix[w].push_back(v);
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::pair<unsigned long int, unsigned long int>> Graph::get_links_vector() {
    return this->links_vector;
}

std::vector<std::vector<unsigned long int>> Graph::get_adj_matrix() {
    return this->adj_matrix;
}

std::vector<unsigned long int> Graph::get_degree_distribution() {
    std::vector<unsigned long int> dist;
    for (unsigned long int i=0; i < this->adj_matrix.size(); i++) {
        dist.push_back(this->adj_matrix[i].size());
    }
    return dist;
}

unsigned long int Graph::get_n() {
    return this->N;
}

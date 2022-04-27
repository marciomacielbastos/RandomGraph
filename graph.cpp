#include "graph.h"

Graph::Graph(){

}

Graph::~Graph() {
    this->get_adj_matrix().clear();
    this->links_vector.clear();
}

Graph::Graph(unsigned long int N){
    std::vector<Rb_tree> adj_matrix(N, Rb_tree());
    this->adj_matrix = adj_matrix;
    this->N = N;
}

void Graph::read_file(std::string filename, char delimiter) {
    this->N = N;
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
    if (this->adj_matrix[v].size() > this->adj_matrix[w].size()) {
        unsigned long int  temp;
        temp = v;
        v = w;
        w = temp;
    }
    return this->adj_matrix[v].is_present(w);
}

bool Graph::link(unsigned long v, unsigned long w) {
    if(!is_connected(v, w)){
        std::pair<unsigned long int, unsigned long int> edge(v, w);
        this->links_vector.push_back(edge);
        this->adj_matrix[v].insert(w);
        this->adj_matrix[w].insert(v);
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::pair<unsigned long int, unsigned long int>> Graph::get_links_vector() {
    return this->links_vector;
}

std::vector<Rb_tree> Graph::get_adj_matrix() {
    return this->adj_matrix;
}

std::vector<unsigned long int> Graph::get_degrees() {
    std::vector<unsigned long int> dist;
    for (unsigned long int i=0; i < this->adj_matrix.size(); i++) {
        dist.push_back(this->adj_matrix[i].size());
    }
    return dist;
}

unsigned long int Graph::get_N() {
    return this->N;
}

void Graph::save_graph(std::string filepath) {
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < this->links_vector.size(); i++ ){
        myfile << this->links_vector[i].first << " " << this->links_vector[i].second << std::endl;
    }
    myfile.close();
}

void Graph::save_degrees(std::string filepath) {
    std::ofstream myfile;
    myfile.open (filepath);
    std::vector<unsigned long int> degrees = get_degrees();
    for(unsigned long int i = 0;  i < degrees.size(); i++ ){
        myfile << degrees[i] << std::endl;
    }
    myfile.close();
}

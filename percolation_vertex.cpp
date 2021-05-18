#include "percolation_vertex.h"

Percolation_vertex::Percolation_vertex(){}

std::vector<unsigned long int> Percolation_vertex::build_identity_vector(unsigned long int N) {
    std::vector<unsigned long int> vertices(N, 0);
    for (unsigned long int i=0; i < N; i++) {
        vertices[i] = i;
    }
    return vertices;
}

std::vector<unsigned long int> Percolation_vertex::define_ticks(double lower_bound, double upper_bound, unsigned long int number_of_ticks, unsigned long int N) {
    std::vector<unsigned long int> ticks;
    double min_number_of_removed_nodes = lower_bound * static_cast<double>(N);
    double max_number_of_removed_nodes = std::ceil(upper_bound * static_cast<double>(N));
    number_of_ticks = static_cast<unsigned long int>(std::min(static_cast<double>(number_of_ticks), (max_number_of_removed_nodes - min_number_of_removed_nodes)));
    double step = static_cast<double>(N) * (upper_bound - lower_bound) / static_cast<double>(number_of_ticks);
    // Round to one decimal point value
    step = (double)((int)(step * 10 + 0.5)) / 10;
    number_of_ticks -= 2;
    ticks.push_back(static_cast<unsigned long int>(max_number_of_removed_nodes));
    for (long int i = number_of_ticks; i >= 0; i--) {
        double tick = min_number_of_removed_nodes + static_cast<double>(i) * step;
        tick = std::ceil(tick);
        ticks.push_back(static_cast<unsigned long int>(tick));
    }    
    return ticks;
}

void Percolation_vertex::smart_pop(std::vector<unsigned long> &vect, unsigned long int idx) {
    vect[idx] = vect.back();
    vect.pop_back();
}

void Percolation_vertex::remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node) {
    for (unsigned long int idx = 0; idx < neighborhood.size(); ++idx) {
        if (neighborhood[idx] == node) {
           smart_pop(neighborhood, idx);
        }
    }
}

double Percolation_vertex::calculate_mean(std::vector<unsigned long int>& values_vector) {
    double mean = 0;
    for (auto value : values_vector) {
        mean += static_cast<double>(value);
    }
    return mean / static_cast<double>(values_vector.size());
}

double Percolation_vertex::calculate_mean_sqr(std::vector<unsigned long int> & values_vector) {
    double mean = 0;
    for (auto value : values_vector) {
        double double_value = static_cast<double>(value);
        mean += double_value * double_value;
    }
    return mean / static_cast<double>(values_vector.size());
}

unsigned long int Percolation_vertex::calculate_maximal_component_size(unsigned long int N, std::vector<std::vector<unsigned long int>> &adj_matrix) {
    UnionFind uf(N);
    unsigned long int v, w;
    for (unsigned long int i = 0; i < N; i++) {
        for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
            v = i;
            w = adj_matrix[i][j];
            if (!adj_matrix[w].empty()) {
                if (v < w) {
                    uf.union_(v, w);
                }
            }
        }
    }
    return uf.get_maximal_component_size();
}

std::vector<unsigned long int> Percolation_vertex::build_degree_vector(std::vector<std::vector<unsigned long int>> &adj_matrix) {
    unsigned long int N = adj_matrix.size();
    std::vector<unsigned long int> degrees(N, 0);
    for (unsigned long int i = 0; i < N; ++i) {
        degrees[i] = adj_matrix[i].size();
    }
    return degrees;
}

double Percolation_vertex::calculate_mr_criterion(std::vector<std::vector<unsigned long int>> &adj_matrix) {
    std::vector<unsigned long int> degrees_vector = build_degree_vector(adj_matrix);
    return calculate_mean_sqr(degrees_vector) / calculate_mean(degrees_vector);
}


void Percolation_vertex::percolate(Graph & G) {
    Uniform uniform_random_variable;
    unsigned long int vertex_idx;
    unsigned long int vertex;
    unsigned long int N = G.get_N();
    unsigned long int size_of_biggest_component;
    unsigned long int neighbor;
    double density_of_biggest_component;
    //Get the adjacent matrix
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    //Get list of nodes
    std::vector<unsigned long int> vertices = build_identity_vector(N);
    while (!vertices.empty()) {
        // Calculate the size of the biggest component
        size_of_biggest_component = calculate_maximal_component_size(N, adj_matrix);
        // Push the density of the biggest component
        density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
        this->result.push_back(density_of_biggest_component);
        vertex_idx = uniform_random_variable.randint(vertices.size());
        vertex = vertices[vertex_idx];
        smart_pop(vertices,vertex_idx);
        while (!adj_matrix[vertex].empty()) {
            neighbor = adj_matrix[vertex].back();
            adj_matrix[vertex].pop_back();
            remove_node_from_neighborhood(adj_matrix[neighbor], vertex);
        }
    }
}

void Percolation_vertex::percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks) {
    Uniform uniform_random_variable;
    double density_of_biggest_component;
    unsigned long int number_of_removed_vertices = 0;
    unsigned long int N = G.get_N();
    unsigned long int vertex;
    unsigned long int vertex_idx;
    unsigned long int neighbor;
    unsigned long int size_of_biggest_component;
    //Define registered ticks
    std::vector<unsigned long int> ticks = define_ticks(lower_bound, upper_bound, number_of_ticks, N);
    //Get the adjacent matrix
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    //Get list of nodes
    std::vector<unsigned long int> vertices = build_identity_vector(N);
    while (true) {
        if (number_of_removed_vertices == ticks.back()) {
            ticks.pop_back();
            // Calculate the size of the biggest component
            size_of_biggest_component = calculate_maximal_component_size(N, adj_matrix);
            // Push the density of the biggest component
            density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
            this->result.push_back(density_of_biggest_component);
            if(ticks.empty()) break;
        }
        vertex_idx = uniform_random_variable.randint(vertices.size());
        vertex = vertices[vertex_idx];
        smart_pop(vertices,vertex_idx);
        while (!adj_matrix[vertex].empty()) {
            neighbor = adj_matrix[vertex].back();
            adj_matrix[vertex].pop_back();
            remove_node_from_neighborhood(adj_matrix[neighbor], vertex);
        }
        number_of_removed_vertices++;
    }
}

std::vector<double> Percolation_vertex::get_result() {
    return this->result;
}

void Percolation_vertex::save (std::string filepath) {
    save(filepath, ",");
}

void Percolation_vertex::save (std::string filepath, std::string separator) {
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < this->result.size(); i++ ){
        if (i == this->result.size() - 1) {
            myfile << this->result[i];
        }
        else {
            myfile << this->result[i] << separator;
        }
    }
    myfile.close();
}

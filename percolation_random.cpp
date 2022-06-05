#include "percolation_random.h"

Percolation_random::Percolation_random(){}

std::vector<unsigned long int> Percolation_random::build_identity_vector(unsigned long int N) {
    std::vector<unsigned long int> vertices(N, 0);
    for (unsigned long int i=0; i < N; i++) {
        vertices[i] = i;
    }
    return vertices;
}

std::vector<unsigned long int> Percolation_random::define_ticks(double lower_bound, double upper_bound, unsigned long int number_of_ticks, unsigned long int N) {
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

void Percolation_random::smart_pop(std::vector<unsigned long> &vect, unsigned long int idx) {
    vect[idx] = vect.back();
    vect.pop_back();
}

void Percolation_random::remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node) {
    for (unsigned long int idx = 0; idx < neighborhood.size(); ++idx) {
        if (neighborhood[idx] == node) {
           smart_pop(neighborhood, idx);
        }
    }
}

double Percolation_random::calculate_mean(std::vector<unsigned long int>& values_vector) {
    double mean = 0;
    for (auto value : values_vector) {
        mean += static_cast<double>(value);
    }
    return mean / static_cast<double>(values_vector.size());
}

double Percolation_random::calculate_mean_sqr(std::vector<unsigned long int> & values_vector) {
    double mean = 0;
    for (auto value : values_vector) {
        double double_value = static_cast<double>(value);
        mean += double_value * double_value;
    }
    return mean / static_cast<double>(values_vector.size());
}

unsigned long int Percolation_random::calculate_maximal_component_size(unsigned long int N, Graph &G) {
    UnionFind uf(N);
    unsigned long int w;
    std::vector<unsigned long int> w_list;
    for (unsigned long int v = 0; v < N; v++) {
        if (!G.is_empty(v)) {
            w_list = G.get_neighbors(v);
            while (!w_list.empty()) {
                w = w_list.back();
                w_list.pop_back();
                if (v < w) {
                    uf.union_(v, w);
                }
            }
        }
    }
    return uf.get_maximal_component_size();
}

double Percolation_random::calculate_mr_criterion(Graph &G) {
    std::vector<unsigned long int> degrees_vector = G.get_degrees();
    return calculate_mean_sqr(degrees_vector) / calculate_mean(degrees_vector);
}

void Percolation_random::percolate(Graph & G) {
    percolate_on_the_interval(G, 0, 1, G.get_N());
}

void Percolation_random::percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks) {
    Uniform uniform_random_variable;
    double density_of_biggest_component;
    double molloy_reed;
    double f = -1;
    unsigned long int number_of_removed_vertices = 0;
    unsigned long int N = G.get_N();
    unsigned long int vertex;
    unsigned long int vertex_idx;
    unsigned long int neighbor;
    std::vector<unsigned long int> neighbors;
    unsigned long int size_of_biggest_component;
    //Define registered ticks
    std::vector<unsigned long int> ticks = define_ticks(lower_bound, upper_bound, number_of_ticks, N);
    //Get list of nodes
    std::vector<unsigned long int> vertices = build_identity_vector(N);
    molloy_reed = calculate_mr_criterion(G);
    while (vertices.size()) {
        if (number_of_removed_vertices == ticks.back()) {
            ticks.pop_back();
            // Calculate the size of the biggest component
            size_of_biggest_component = calculate_maximal_component_size(N, G);
            // Push the density of the biggest component
            density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
            this->result.push_back(density_of_biggest_component);
        }
        vertex_idx = uniform_random_variable.randint(vertices.size());
        vertex = vertices[vertex_idx];
        smart_pop(vertices,vertex_idx);
        neighbors = G.get_neighbors(vertex);
        while (!neighbors.empty()) {
            neighbor = neighbors.back();
            neighbors.pop_back();
            G.del_link(vertex, neighbor);
        }
        number_of_removed_vertices++;
        if (molloy_reed > 2) {
            molloy_reed = calculate_mr_criterion(G);
        }
        if (molloy_reed <= 2 && f < 0) {
            f = static_cast<double>(number_of_removed_vertices) / static_cast<double>(N);
        }
    }
    if (f < 0) {
        f = 1;
    }
    this->result.push_back(f);
}

/*******************************************************************/

/*******************************************************************/

std::vector<double> Percolation_random::get_result() {
    return this->result;
}

void Percolation_random::save (std::string filepath) {
    save(filepath, ",");
}

void Percolation_random::save (std::string filepath, std::string separator) {
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < this->result.size(); i++ ){
        if (i == this->result.size() - 1) {
            myfile << this->result[i] << std::endl;
        }
        else {
            myfile << this->result[i] << separator;
        }
    }
    myfile.close();
}

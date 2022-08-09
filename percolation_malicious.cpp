#include "percolation_malicious.h"

Percolation_malicious::Percolation_malicious() {}

void Percolation_malicious::percolate(Graph & G) {
    this->percolate_on_the_interval(G,0,1,200);
}

//void Percolation_malicious::percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks) {
//    double density_of_biggest_component;
//    double molloy_reed;
//    double f = -1;
//    unsigned long int number_of_removed_vertices = 0;
//    unsigned long int N = G.get_N();
//    unsigned long int neighbor;
//    unsigned long int root;
//    unsigned long int size_of_biggest_component;
//    std::vector<unsigned long int> neighbors;
//    //Define registered ticks
//    std::vector<unsigned long int> ticks = define_ticks(lower_bound, upper_bound, number_of_ticks, N);
//    // Build a degree vector
//    std::vector<unsigned long int> degrees = G.get_degrees();
//    // Build the heap using the degrees vector
//    Heap_desc<unsigned long int> heap(degrees);
//    while (true) {
//        if (number_of_removed_vertices == ticks.back()) {
//            ticks.pop_back();
//            // Calculate the size of the biggest component
//            size_of_biggest_component = calculate_maximal_component_size(N, G);
//            // Push the density of the biggest component
//            density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
//            this->result.push_back(density_of_biggest_component);
//            if(ticks.empty()) break;
//        }
//        root = heap.pop();
//        /*****************************/
//        neighbors = G.get_neighbors(root);
//        while (!neighbors.empty()) {
//            neighbor = neighbors.back();
//            neighbors.pop_back();
//            if (!G.is_empty(neighbor)) {
//                heap.update(neighbor);
//            }
//            G.del_link(root, neighbor);
//        }
//        number_of_removed_vertices++;
//        if (molloy_reed > 2) {
//            molloy_reed = calculate_mr_criterion(G);
//        }
//        if (molloy_reed <= 2 && f < 0) {
//            f = static_cast<double>(number_of_removed_vertices) / static_cast<double>(N);
//        }
//    }
//    if (f < 0) {
//        f = 1;
//    }
//    this->result.push_back(f);
//}

void Percolation_malicious::percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks) {
    double density_of_biggest_component;
    double molloy_reed;
    double f = -1;
    unsigned long int number_of_removed_vertices = 0;
    unsigned long int N = G.get_N();
    unsigned long int vertex;
    unsigned long int neighbor;
    std::vector<unsigned long int> neighbors;
    unsigned long int size_of_biggest_component;
    //Define registered ticks
    std::vector<unsigned long int> ticks = define_ticks(lower_bound, upper_bound, number_of_ticks, N);
    std::vector<unsigned long int> degrees = G.get_degrees();
    // Build the heap using the degrees vector
    Heap_desc<unsigned long int> heap(degrees);
    molloy_reed = calculate_mr_criterion(G);
    while (!heap.is_empty()) {
        if (number_of_removed_vertices == ticks.back()) {
            ticks.pop_back();
            // Calculate the size of the biggest component
            size_of_biggest_component = calculate_maximal_component_size(N, G);
            // Push the density of the biggest component
            density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
            this->result.push_back(density_of_biggest_component);
        }
        vertex = heap.pop();
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


std::vector<double> Percolation_malicious::get_result() {
    return this->result;
}

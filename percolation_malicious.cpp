//#include "percolation_malicious.h"

//Percolation_malicious::Percolation_malicious() {}

//std::vector<unsigned long int> Percolation_malicious::build_degree_vector(std::vector<std::vector<unsigned long int>> &adj_matrix) {
//    unsigned long int N = adj_matrix.size();
//    std::vector<unsigned long int> degrees(N, 0);
//    for (unsigned long int i = 0; i < N; ++i) {
//        degrees[i] = adj_matrix[i].size();
//    }
//    return degrees;
//}

//void Percolation_malicious::percolate(Graph & G) {
//    double density_of_biggest_component;
//    unsigned long int N = G.get_N();
//    unsigned long int neighbor;
//    unsigned long int root;
//    unsigned long int size_of_biggest_component;
//    //Get the adjacent matrix
//    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
//    // Build a degree vector
//    std::vector<unsigned long int> degrees = build_degree_vector(adj_matrix);
//    // Build the heap using the degrees vector
//    Heap_desc<unsigned long int> heap(degrees);
//    while (true) {
//        // Calculate the size of the biggest component
//        size_of_biggest_component = calculate_maximal_component_size(N, adj_matrix);
//        // Push the density of the biggest component
//        density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
//        this->result.push_back(density_of_biggest_component);
//        root = heap.pop();
//        while (!adj_matrix[root].empty()) {
//            neighbor = adj_matrix[root].back();
//            if (!adj_matrix[neighbor].empty()) {
//                heap.update(neighbor);
//            }
//            adj_matrix[root].pop_back();
//            remove_node_from_neighborhood(adj_matrix[neighbor], root);
//        }
//    }
//}

//void Percolation_malicious::percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks) {
//    double density_of_biggest_component;
//    unsigned long int number_of_removed_vertices = 0;
//    unsigned long int N = G.get_N();
//    unsigned long int neighbor;
//    unsigned long int root;
//    unsigned long int size_of_biggest_component;
//    //Define registered ticks
//    std::vector<unsigned long int> ticks = define_ticks(lower_bound, upper_bound, number_of_ticks, N);
//    //Get the adjacent matrix
//    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
//    // Build a degree vector
//    std::vector<unsigned long int> degrees = build_degree_vector(adj_matrix);
//    // Build the heap using the degrees vector
//    Heap_desc<unsigned long int> heap(degrees);
//    while (true) {
//        if (number_of_removed_vertices == ticks.back()) {
//            ticks.pop_back();
//            // Calculate the size of the biggest component
//            size_of_biggest_component = calculate_maximal_component_size(N, adj_matrix);
//            // Push the density of the biggest component
//            density_of_biggest_component = static_cast<double>(size_of_biggest_component)/ static_cast<double>(N);
//            this->result.push_back(density_of_biggest_component);
//            if(ticks.empty()) break;
//        }
//        root = heap.pop();
//        while (!adj_matrix[root].empty()) {
//            neighbor = adj_matrix[root].back();
//            if (!adj_matrix[neighbor].empty()) {
//                heap.update(neighbor);
//            }
//            adj_matrix[root].pop_back();
//            remove_node_from_neighborhood(adj_matrix[neighbor], root);
//        }
//        number_of_removed_vertices++;
//    }
//}

//void Percolation_malicious::percolate_molloy_reed(Graph & G) {
//    double molloy_reed;
//    double f;
//    unsigned long int number_of_removed_vertices = 0;
//    unsigned long int N = G.get_N();
//    unsigned long int neighbor;
//    unsigned long int root;
//    //Get the adjacent matrix
//    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
//    // Build a degree vector
//    std::vector<unsigned long int> degrees = build_degree_vector(adj_matrix);
//    // Build the heap using the degrees vector
//    Heap_desc<unsigned long int> heap(degrees);
//    molloy_reed = calculate_mr_criterion(adj_matrix);
//    while (molloy_reed > 2) {
//        root = heap.pop();
//        while (!adj_matrix[root].empty()) {
//            neighbor = adj_matrix[root].back();
//            if (!adj_matrix[neighbor].empty()) {
//                heap.update(neighbor);
//            }
//            adj_matrix[root].pop_back();
//            remove_node_from_neighborhood(adj_matrix[neighbor], root);
//        }
//        number_of_removed_vertices++;
//        f = static_cast<double>(number_of_removed_vertices) / static_cast<double>(N);
//        molloy_reed = calculate_mr_criterion(adj_matrix);
//    }
//    this->result.push_back(f);
//}


//std::vector<double> Percolation_malicious::get_result() {
//    return this->result;
//}

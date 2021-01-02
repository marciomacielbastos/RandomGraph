#include "percolation_degree.h"

Percolation_degree::Percolation_degree() {

}

void Percolation_degree::smart_pop(std::vector<unsigned long> &vect, unsigned long int idx) {
    vect[idx] = vect.back();
    vect.pop_back();
}

void Percolation_degree::remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node) {
    for (unsigned long int idx = 0; idx < neighborhood.size(); ++idx) {
        if (neighborhood[idx] == node) {
           smart_pop(neighborhood, idx);
        }
    }
}

void Percolation_degree::remove_node(std::vector<std::vector<unsigned long>> &adj_matrix, unsigned long int node) {
    unsigned long int neighbor;
    while (!adj_matrix[node].empty()) {
        neighbor = adj_matrix[node].back();
        adj_matrix[node].pop_back();
        remove_node_from_neighborhood(adj_matrix[neighbor], node);
    }
}

void Percolation_degree::malicious_attack(Graph & G) {
    unsigned long int N = G.get_n();
    unsigned long int root;
    unsigned long int neighbor;
    std::vector<double> network_size;
    std::vector<unsigned long int> degrees;
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    double size_of_max_comp;
    for (unsigned long int i = 0; i < N; ++i) {
        degrees.push_back(adj_matrix[i].size());
    }

    Heap_desc<unsigned long int> heap(degrees);
    unsigned long int n = 0;
    UnionFind uf(N);
    for (unsigned long int i = 0; i < N; i++) {
        for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
            if (i < adj_matrix[i][j]) {
                uf.union_(i, adj_matrix[i][j]);
            }
        }
    }
    size_of_max_comp = static_cast<double>(uf.get_size_of_max_comp());
    network_size.push_back(size_of_max_comp/ static_cast<double>(N));
    while (!heap.is_empty()) {
        root = heap.pop();
        n++;

        while (!adj_matrix[root].empty()) {
            neighbor = adj_matrix[root].back();
            heap.update(neighbor);
            adj_matrix[root].pop_back();
            remove_node_from_neighborhood(adj_matrix[neighbor], root);
        }
        if (heap.size() == 1) {
            network_size.push_back(1.0 / static_cast<double>(N));
            heap.pop();
            n++;
        }

        UnionFind uf(N);
        for (unsigned long int i = 0; i < N; i++) {
            for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
                if (i < adj_matrix[i][j]) {
                    uf.union_(i, adj_matrix[i][j]);
                }
            }
        }
        size_of_max_comp = static_cast<double>(uf.get_size_of_max_comp());
        network_size.push_back(size_of_max_comp/ static_cast<double>(N));
    }
    network_size.push_back(0.0 / static_cast<double>(N));
    this->result = network_size;
}

std::vector<double> Percolation_degree::get_result() {
    return this->result;
}

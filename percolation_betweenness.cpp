#include "percolation_betweenness.h"

Percolation_betweenness::Percolation_betweenness() {
    this->mean_l = 0;
}

void Percolation_betweenness::remove_node(unsigned long int idx, std::vector<std::vector<unsigned long int>> & adj_matrix) {
    unsigned long int j, k;
    while (!adj_matrix[idx].empty()) {
        j = adj_matrix[idx].back();
        adj_matrix[idx].pop_back();
        k = 0;
        while (idx != adj_matrix[j][k]) {
            k++;
        }
        adj_matrix[j][k] = adj_matrix[j].back();
        adj_matrix[j].pop_back();
    }
}

unsigned long int Percolation_betweenness::get_biggest(std::vector<std::vector<unsigned long int>> & adj_matrix) {
    unsigned long int N = adj_matrix.size();
    unsigned long int k;
    UnionFind uf(N);
    for (unsigned long int i = 0; i < N; i++) {
        for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
            k = adj_matrix[i][j];
            if (i < k) {
                uf.union_(i, k);
            }
        }
    }
    return uf.get_biggest_comp_id_size().second;
}

void Percolation_betweenness::percolation_betweenness(Graph &G, unsigned long int n_threads) {

    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    unsigned long int N = G.get_n();
    unsigned long int size = get_biggest(adj_matrix);
    unsigned long int i = 0;

    std::vector<double> result;

    Betweenness b(G, n_threads);
    this->mean_l = b.mean_l();
    std::vector<double> betweenness = b.betweenness_centrality();
    Heap_desc<double> heap(betweenness);

    result.push_back(static_cast<double>(size) / N);

    while (!heap.is_empty()) {
        if (!i % 100) {
            Betweenness btween(adj_matrix, n_threads);
            betweenness = btween.betweenness_centrality();
            Heap_desc<double> hp(betweenness);
            heap = hp;
        }
        unsigned long int root = heap.pop();
        remove_node(root, adj_matrix);
        size = get_biggest(adj_matrix);
        i++;
        result.push_back(static_cast<double>(size) / N);
    }
    this->result = result;
}

std::vector<double> Percolation_betweenness::get_result() {
    return this->result;
}

double Percolation_betweenness::get_mean_l() {
    return this->mean_l;
}

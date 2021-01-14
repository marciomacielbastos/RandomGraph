#include "percolation_kcore.h"

Percolation_kcore::Percolation_kcore() : Percolation("kcore") {}

void Percolation_kcore::percolate(Graph & G) {
    unsigned long int size = G.get_n();
    unsigned long int root;
    std::vector<double> k_size;
    std::vector<unsigned long int> degrees;
    std::vector<unsigned long int> neighborhood;

    for (unsigned long int i = 0; i < size; ++i) {
        degrees.push_back(G.get_adj_matrix()[i].size());
    }

    Heap_asc<unsigned long int> heap(degrees);
    unsigned long int k = 1;
    unsigned long int root_degree = heap.get_root_value();

    while (!heap.is_empty()) {
        while (root_degree <= k && heap.size() > 1) {
            root = heap.pop();
            neighborhood = G.get_adj_matrix()[root];
            for (unsigned long int i = 0; i < neighborhood.size(); ++i) {
                heap.update(neighborhood[i]);
            }
            root_degree = heap.get_root_value();
            if (heap.size() == 1) {
                root = heap.pop();
            }
        }
        k_size.push_back(static_cast<double>(heap.size()));
        k++;
    }
    this->result = k_size;
}

std::vector<double> Percolation_kcore::get_result() {
    return this->result;
}

std::vector<double> Percolation_kcore::get_other_result() {
    return this->other_result;
}

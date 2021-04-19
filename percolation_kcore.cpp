#include "percolation_kcore.h"

Percolation_kcore::Percolation_kcore() {
}

Percolation_kcore::Percolation_kcore(Graph G) {
    this->G = G;
}

std::vector<unsigned long int> Percolation_kcore::get_degrees() {
    unsigned long int size = this->G.get_n();
    std::vector<unsigned long int> degrees;
    for (unsigned long int i = 0; i < size; ++i) {
        degrees.push_back(this->G.get_adj_matrix()[i].size());
    }
    return degrees;
}

void Percolation_kcore::update_neighborhood(unsigned long int vertex, Heap_asc<unsigned long int> & heap) {
    std::vector<unsigned long int> neighborhood = G.get_adj_matrix()[vertex];
    for (unsigned long int i = 0; i < neighborhood.size(); ++i) {
        heap.update(neighborhood[i]);
    }
}

void  Percolation_kcore::peel_k_layer(unsigned long k, Heap_asc<unsigned long int> & heap) {
    unsigned long int root_degree = heap.get_root_value();
    unsigned long int root;
    while (root_degree <= k) {
        root = heap.pop();
        update_neighborhood(root, heap);
        if (heap.is_empty()) {
            break;
        }
        else {
            root_degree = heap.get_root_value();
        }
    }
}

void Percolation_kcore::percolate() {
    unsigned long int k = 1;
    std::vector<unsigned long int> degrees = get_degrees();
    Heap_asc<unsigned long int> heap(degrees);
    while(!heap.is_empty()) {
        peel_k_layer(k, heap);
        this->result.push_back(static_cast<double>(heap.size()));
        k++;
    }
}

void Percolation_kcore::percolate(Graph &G) {
    this->G = G;
    percolate();
}

std::vector<double> Percolation_kcore::get_result() {
    return this->result;
}


//void Percolation_kcore::percolate(Graph & G) {
//    unsigned long int size = G.get_n();
//    unsigned long int root;
//    std::vector<double> k_size;
//    std::vector<unsigned long int> degrees;
//    std::vector<unsigned long int> neighborhood;

//    for (unsigned long int i = 0; i < size; ++i) {
//        degrees.push_back(G.get_adj_matrix()[i].size());
//    }

//    Heap_asc<unsigned long int> heap(degrees);
//    unsigned long int k = 1;
//    unsigned long int root_degree = heap.get_root_value();

//    while (!heap.is_empty()) {
//        while (root_degree <= k) {
//            root = heap.pop();
//            neighborhood = G.get_adj_matrix()[root];
//            for (unsigned long int i = 0; i < neighborhood.size(); ++i) {
//                heap.update(neighborhood[i]);
//            }
//            if (heap.is_empty()) {
//                break;
//            }
//            else {
//                root_degree = heap.get_root_value();
//            }
//        }
//        k_size.push_back(static_cast<double>(heap.size()));
//        k++;
//    }
//    this->result = k_size;
//}

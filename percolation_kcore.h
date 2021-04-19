#ifndef KCORE_DECOMPOSITION_H
#define KCORE_DECOMPOSITION_H
#include <vector>
#include <heap_asc.h>
#include <graph.h>
#include <percolation.h>

class Percolation_kcore : public Percolation {
protected:
    Graph G;
    std::vector<unsigned long int> get_degrees();
    void update_neighborhood(unsigned long int vertex, Heap_asc<unsigned long int> & heap);
    void peel_k_layer(unsigned long k, Heap_asc<unsigned long int> & heap);
public:
    Percolation_kcore();
    Percolation_kcore(Graph G);
    void percolate();
    void percolate(Graph & G);
    std::vector<double> get_result();
};

#endif // KCORE_DECOMPOSITION_H

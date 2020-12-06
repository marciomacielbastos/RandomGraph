#ifndef KCORE_DECOMPOSITION_H
#define KCORE_DECOMPOSITION_H
#include <vector>
#include <heap_asc.h>
#include <graph.h>

class Percolation_kcore {
private:
    std::vector<double> result;

public:
    Percolation_kcore();
    void kcore_decomposition(Graph & G);
    std::vector<double> get_result();
};

#endif // KCORE_DECOMPOSITION_H

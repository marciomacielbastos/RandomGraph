#ifndef KCORE_DECOMPOSITION_H
#define KCORE_DECOMPOSITION_H
#include <vector>
#include <heap_asc.h>
#include <graph.h>
#include <percolation.h>

class Percolation_kcore : public Percolation {
public:
    Percolation_kcore();
    void percolate(Graph & G);
    std::vector<double> get_result();
    std::vector<double> get_other_result();
};

#endif // KCORE_DECOMPOSITION_H

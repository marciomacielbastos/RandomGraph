#ifndef PERCOLATION_BIGGEST_DEGREE_H
#define PERCOLATION_BIGGEST_DEGREE_H
#include <heap_desc.h>
#include <unionfind.h>
#include <vector>
#include <graph.h>
#include <percolation_random.h>
#include <cmath>


class Percolation_malicious : public Percolation_random {
public:
    Percolation_malicious();
    void percolate(Graph & G);
    void percolate_on_the_interval(Graph & G, double lower_bound, double upper_bound, unsigned long int number_of_ticks);
    std::vector<double> get_result();
};

#endif // PERCOLATION_BIGGEST_DEGREE_H

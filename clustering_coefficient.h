#ifndef CLUSTERING_COEFFICIENT_H
#define CLUSTERING_COEFFICIENT_H
#include <vector>
#include <string>

#include <graph.h>


class Clustering_coefficient {
private:
    double mean(std::vector<const unsigned long int> &degrees);
    double square_mean(const std::vector<unsigned long int> &degrees);

public:
    Clustering_coefficient();
    double calculate_cluster_coefficient(Graph G);
    double calculate_cluster_coefficient(std::string filepath);
};

#endif // CLUSTERING_COEFFICIENT_H

#ifndef BFS_H
#define BFS_H
#include <vector>
#include <queue>
#include "uniform.h"

class Bfs
{
private:
    double average_path;

public:
    Bfs();
    unsigned long int smart_pop(std::vector<unsigned long int> &list, unsigned long int idx);
    double bfs(unsigned long int root, std::vector<std::vector<unsigned long int>> adj_matrix);
    double avg_geo_dist(unsigned long int number_of_samples, std::vector<std::vector<unsigned long int>> adj_matrix);
};

#endif // BFS_H

#ifndef MOUNT_NETWORK_H
#define MOUNT_NETWORK_H
#include <distribution.h>
#include <vector>
#include <queue>
#include <deque>
#include <uniform.h>
#include <bitset>


class Graph{
private:
    std::vector<std::pair<unsigned long int, unsigned long int>> link_list;
    std::vector<std::vector<unsigned long int>> adj_matrix;

public:
    Graph();
    Graph(unsigned long int N);
//    Graph(Graph g);
    bool is_connected(unsigned long v, unsigned long w);
    bool link(unsigned long v, unsigned long w);
    std::vector<std::pair<unsigned long int, unsigned long int>> get_link_list();
    std::vector<unsigned long int> get_real_dist();
};

#endif // MOUNT_NETWORK_H

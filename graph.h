#ifndef MOUNT_NETWORK_H
#define MOUNT_NETWORK_H
#include <distribution.h>
#include <vector>
#include <queue>
#include <deque>
#include <uniform.h>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Graph {
private:
    std::vector<std::pair<unsigned long int, unsigned long int>> link_list;
    std::vector<std::vector<unsigned long int>> adj_matrix;
    unsigned long int N;

public:
    Graph();
    Graph(unsigned long int N);
    void read_file(std::string filename, char delim);
//    Graph(Graph g);
    bool is_connected(unsigned long v, unsigned long w);
    bool link(unsigned long v, unsigned long w);
    unsigned long int get_n();
    std::vector<std::pair<unsigned long int, unsigned long int>> get_link_list();
    std::vector<std::vector<unsigned long int>> get_adj_matrix();
    std::vector<unsigned long int> get_degree_distribution();

};

#endif // MOUNT_NETWORK_H

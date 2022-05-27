#ifndef MOUNT_NETWORK_H
#define MOUNT_NETWORK_H
#include <vector>
#include <queue>
#include <deque>
#include <uniform.h>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <probability_distribution.h>
#include <rb_tree.h>

class Graph {
private:
    std::vector<std::pair<unsigned long int, unsigned long int>> links_vector;
//    std::vector<std::vector<unsigned long int>> adj_matrix;
    std::vector<Rb_tree> adj_matrix;
    unsigned long int N;
    std::vector<Rb_tree> get_adj_matrix();

public:
    Graph();
    ~Graph();
    Graph(unsigned long int N);
    void read_file(std::string filename, char delimiter);
    bool is_connected(unsigned long v, unsigned long w);
    bool is_empty(unsigned long int v);
    bool link(unsigned long v, unsigned long w);
    void del_link(unsigned long v, unsigned long w);
    unsigned long int get_N();
    std::vector<std::pair<unsigned long int, unsigned long int>> get_links_vector();
    std::vector<unsigned long int> get_degrees();
    std::vector<unsigned long int> get_neighbors(unsigned long int v);
    void save_graph(std::string filepath);
    void save_degrees(std::string filepath);
};

#endif // MOUNT_NETWORK_H

#ifndef UNIONFIND_H
#define UNIONFIND_H
#include <map>
#include <vector>
#include <stack>

class UnionFind {
private:
    std::vector<unsigned long int> root;
    std::vector<unsigned long int> sizes;
    std::vector<unsigned long int> number_of_clusters_per_size;
    std::pair<unsigned long int, unsigned long int> maximal_component_root_size_pair;
    unsigned long int count;
public:
    UnionFind();
    UnionFind(unsigned long int N);
    void union_(unsigned long int v, unsigned long int w);
    unsigned long int find(unsigned long int v);
    bool is_connected(unsigned long int v, unsigned long int w);
    std::vector<unsigned long int> get_size_of_components();
    unsigned long int get_size_of_component(unsigned long int v);
    unsigned long int get_number_of_components();
    unsigned long int get_maximal_component_size();
    std::pair<unsigned long int, unsigned long int> get_maximal_component_root_size_pair();
};

#endif // UNIONFIND_H

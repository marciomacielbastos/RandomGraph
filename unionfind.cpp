#include "unionfind.h"

UnionFind::UnionFind(){

}

UnionFind::UnionFind(unsigned long int N){
    std::pair<unsigned long int, unsigned long int> bigger (0, 1);
    this->maximal_component_root_size_pair =  bigger;
    this->count = N;
    for(unsigned long int i=0; i < N; i++){
        this->root.push_back(i);
    }
    for(unsigned int i=0; i < N;i++) {
        this->sizes.push_back(1);
        this->number_of_clusters_per_size.push_back(0);
    }
    this->number_of_clusters_per_size[0] = N;
}

unsigned long int UnionFind::get_number_of_components(){
    return count;
}

unsigned long int UnionFind::find(unsigned long int v){
    std::stack<unsigned long int> update;
    unsigned long int w;
    update.push(v);
    while (v != this->root[v]) {
        v = this->root[v];
        update.push(v);
    }
    while (!update.empty()) {
        w = update.top();
        update.pop();
        this->root[w] = v;
    }
    return v;
}

void UnionFind::union_(unsigned long int v, unsigned long int w){
    unsigned long int vRoot = this->find(v);
    unsigned long int wRoot = this->find(w);
    unsigned long int temp;
    unsigned long int vsize, wsize;
    if(vRoot == wRoot) return;
    else if (this->sizes[vRoot] < this->sizes[wRoot]) {
        temp = vRoot;
        vRoot = wRoot;
        wRoot = temp;
    }
    this->root[wRoot] = vRoot;
    vsize = this->sizes[vRoot];
    wsize = this->sizes[wRoot];
    this->number_of_clusters_per_size[vsize-1]--;
    this->number_of_clusters_per_size[wsize-1]--;
    this->sizes[vRoot] += this->sizes[wRoot];
    vsize = this->sizes[vRoot];
    this->number_of_clusters_per_size[vsize-1]++;
    if(vsize > this->maximal_component_root_size_pair.second){
        this->maximal_component_root_size_pair.first = vRoot;
        this->maximal_component_root_size_pair.second = vsize;
    }
    this->count--;
}

bool UnionFind::is_connected(unsigned long int v, unsigned long int w){
    return (this->find(v) == this->find(w));
}

unsigned long int UnionFind::get_size_of_component(unsigned long int v){
    return this->sizes[v];
}

std::vector<unsigned long int> UnionFind::get_size_of_components(){
    return this->number_of_clusters_per_size;
}

unsigned long int UnionFind::get_maximal_component_size(){
    return this->maximal_component_root_size_pair.second;
}

/**
* Return the id of the node and the size of the biggest network component.
* The id is linked to the original node after successive edge addings.
 * @param Null.
 * @return a pair(id, size).
 */
std::pair<unsigned long int, unsigned long int> UnionFind::get_maximal_component_root_size_pair(){
    return this->maximal_component_root_size_pair;
}

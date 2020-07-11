#include "bfs.h"

Bfs::Bfs() {

}

unsigned long int Bfs::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

double Bfs::bfs(unsigned long s, std::vector<std::vector<unsigned long int>> adj_matrix) {
    double dsum = 0;
    double dist = 0;
    unsigned long int d1 = 0;
    unsigned long int d2 = 1;
    std::queue<unsigned long int> q;
    q.push(s);
    double size = static_cast<double>(adj_matrix.size());
    std::vector<bool> marked(adj_matrix.size());
    marked[s] = true;
    while(!q.empty()){
        if(d1 == 0){
            d1 = d2;
            d2 = 0;
            dist++;
        }
        unsigned long int v = q.front();
        q.pop();
        d1--;
        for(unsigned long int w : adj_matrix[v]){
            if(!marked[w]){
                d2++;
                dsum += dist;
                marked[w] = true;
                q.push(w);
            }
        }
    }
    return dsum/size;
}

double Bfs::avg_geo_dist(unsigned long int number_of_samples, std::vector<std::vector<unsigned long int>> adj_matrix){
    unsigned long s;
    Uniform u;
    unsigned long int N = adj_matrix.size();
    double sum = 0;
    double total = static_cast<double>(number_of_samples);

    std::vector<unsigned long int> list_of_nodes(N);
    for(unsigned long int i=0; i<N; i++){
            list_of_nodes[i] = i;
    }

    while(number_of_samples){
        unsigned long int i = u.randint(list_of_nodes.size());
        s = smart_pop(list_of_nodes, i);
        sum += bfs(s, adj_matrix);
        number_of_samples--;
    }
    return sum/total;
}

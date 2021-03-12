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
    double size = static_cast<double>(adj_matrix.size());
    unsigned long int d1 = 0;
    unsigned long int d2 = 1;

    std::queue<unsigned long int> q;
    q.push(s);

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

double Bfs::avg_geo_dist(unsigned long int number_of_samples, std::vector<std::vector<unsigned long int>> adj_matrix) {
    Uniform u;
    unsigned long s;
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

void Bfs::start_vectors(unsigned long N){
    std::vector<double> sigma(N);
    this->sigma = sigma;
    std::vector<unsigned long int> S;
    this->S = S;
    std::vector<std::vector<unsigned long int>> P(N);
    this->P = P;
}

// modified from Eppstein
double Bfs::bfs(Graph & G, unsigned long int s) {
    unsigned long int N = G.get_adj_matrix().size();
    unsigned long int v;

    std::vector<unsigned long int> D(N, N+1);
    std::queue<unsigned long int> q;
    std::vector<unsigned long int> neighbourhood;
    double dsum = 0;

    start_vectors(N);
    this->sigma[s] = 1.0;
    D[s] = 0;
    q.push(s);

    while (!q.empty()) {
        v = q.front();
        this->S.push_back(v);
        q.pop();
        neighbourhood = G.get_adj_matrix()[v];
        for (auto w : neighbourhood) {

            if (D[w] > N) {
                D[w] = D[v] + 1;
                q.push(w);

                dsum += D[w];
            }
            // If shortest path through w, count them all...
            if(D[w] == D[v] + 1) {
                this->sigma[w] += sigma[v];
                this->P[w].push_back(v);
            }
        }
    }
    this->average_path = dsum/static_cast<double>(N - 1);
    return this->average_path;
}

// modified from Eppstein
double Bfs::bfs(Graph & G, unsigned long int s, std::vector<std::vector<unsigned long int>> &p) {
    unsigned long int N = G.get_adj_matrix().size();
    unsigned long int v;
    std::vector<unsigned long int> D(N, N+1);
    std::queue<unsigned long int> q;
    std::vector<unsigned long int> neighbourhood;
    double dsum = 0;

    start_vectors(N);
    this->sigma[s] = 1.0;
    D[s] = 0;
    q.push(s);

    while (!q.empty()) {
        v = q.front();
        this->S.push_back(v);
        q.pop();
        neighbourhood = G.get_adj_matrix()[v];
        for (auto w : neighbourhood) {

            if (D[w] > N) {
                D[w] = D[v] + 1;
                q.push(w);

                dsum += D[w];
            }
            // If shortest path through w, count them all...
            if(D[w] == D[v] + 1) {
                this->sigma[w] += sigma[v];
                if (p[w].empty()) p[w].push_back(static_cast<unsigned long int>(this->sigma[w]));
                else {
                    p[w].push_back(static_cast<unsigned long int>(this->sigma[w]) + p[w].back());
                }
                this->P[w].push_back(v);
            }
        }
    }
    this->average_path = dsum/static_cast<double>(N - 1);
    return this->average_path;
}

// modified from Eppstein
double Bfs::bfs(std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int s) {
    unsigned long int N = adj_matrix.size();
    unsigned long int v;

    std::vector<unsigned long int> D(N, N+1);
    std::queue<unsigned long int> q;
    double dsum = 0;

    start_vectors(N);
    this->sigma[s] = 1.0;
    D[s] = 0;
    q.push(s);

    while (!q.empty()) {
        v = q.front();
        this->S.push_back(v);
        q.pop();
        for (auto w : adj_matrix[v]) {

            if (D[w] > N) {
                D[w] = D[v] + 1;
                q.push(w);

                dsum += D[w];
            }
            // If shortest path through w, count them all...
            if(D[w] == D[v] + 1) {
                this->sigma[w] += sigma[v];
                this->P[w].push_back(v);
            }
        }
    }
    this->average_path = dsum/static_cast<double>(N - 1);
    return this->average_path;
}

void Bfs::bfs(Graph & G, unsigned long int s, double & l) {
    unsigned long int N = G.get_adj_matrix().size();
    unsigned long int v;

    std::vector<unsigned long int> D(N, N+1);
    std::queue<unsigned long int> q;
    std::vector<unsigned long int> neighbourhood;
    double dsum = 0;

    start_vectors(N);
    this->sigma[s] = 1.0;
    D[s] = 0;
    q.push(s);

    while (!q.empty()) {
        v = q.front();
        this->S.push_back(v);
        q.pop();
        neighbourhood = G.get_adj_matrix()[v];
        for (auto w : neighbourhood) {

            if (D[w] > N) {
                D[w] = D[v] + 1;
                q.push(w);

                dsum += D[w];
            }
            // If shortest path through w, count them all...
            if(D[w] == D[v] + 1) {
                this->sigma[w] += sigma[v];
                this->P[w].push_back(v);
            }
        }
    }
    l = dsum/static_cast<double>(N - 1);
}

std::vector<std::vector<unsigned long int>> Bfs::getP() {
    return this->P;
}

std::vector<unsigned long int> Bfs::getS() {
    return this->S;
}

std::vector<double> Bfs::get_sigma() {
    return this->sigma;
}

double Bfs::get_l() {
    return this->average_path;
}

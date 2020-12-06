#include "betweenness.h"

Betweenness::Betweenness() {

}

Betweenness::Betweenness(Graph &G, unsigned long int n_threads) {
    this->n_threads = n_threads;
    this->average_path = 0;
    unsigned long int N = G.get_adj_matrix().size();
    std::vector<double> b(N);
    this->betweenness = b;  // b[v]=0 for v in G
    betweenness_centrality(G);
}

Betweenness::Betweenness(std::vector<std::vector<unsigned long int>> &adj_matrix,
                         unsigned long int n_threads) {
    this->n_threads = n_threads;
    this->average_path = 0;
    unsigned long int N = adj_matrix.size();
    std::vector<double> b(N);
    this->betweenness = b;  // b[v]=0 for v in G
    betweenness_centrality(adj_matrix);
}

void Betweenness::accumulation(std::vector<std::vector<unsigned long int>> &P,
                               std::vector<unsigned long int> &S,
                               std::vector<double> &sigma,
                               unsigned long int s) {

    std::vector<double> delta(P.size());
    while(!S.empty()) {
        unsigned long int w = S.back();
        S.pop_back();

        for(unsigned long int v : P[w]) {
            delta[v] += (sigma[v] / sigma[w]) * (1 + delta[w]);
        }

        if (w != s) {
//            std::lock_guard<std::mutex> lock (this->mutex);
            std::unique_lock<std::mutex> lock (this->mutex);
            this->betweenness[w] += delta[w];
            lock.unlock();
//            this->betweenness[w] += delta[w];
        }
    }
}

void Betweenness::betweenness_centrality(Graph & G) {
    unsigned long int N = G.get_adj_matrix().size();
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < N; i++) {
        if ((i % n_threads) == 0) {
            betweenness_centrality_of_s(G,i);
            while (!threads.empty()) {
                threads.back().join();
                threads.pop_back();
            }          
        }
        else {
            std::thread t (&Betweenness::betweenness_centrality_of_s, this, std::ref(G), i);
            threads.push_back(std::move(t));
        }
    }
    while (!threads.empty()) {
        threads.back().join();
        threads.pop_back();
    }
    this->average_path /= N;
}

void Betweenness::betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix) {
    unsigned long int N = adj_matrix.size();
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < N; i++) {
        if ((i % n_threads) == 0) {
            betweenness_centrality_of_i(adj_matrix,i);
            while (!threads.empty()) {
                threads.back().join();
                threads.pop_back();
            }
        }
        else {
            std::thread t (&Betweenness::betweenness_centrality_of_i, this, std::ref(adj_matrix), i);
            threads.push_back(std::move(t));
        }
    }
    while (!threads.empty()) {
        threads.back().join();
        threads.pop_back();
    }
    this->average_path /= N;
}

void Betweenness::betweenness_centrality_of_s(Graph & G, unsigned long int s) {
    Bfs search;
    search.bfs(G, s);
    std::unique_lock<std::mutex> lock (this->mutex);
    this->average_path += search.get_l();
    lock.unlock();
//  accumulation
    std::vector<std::vector<unsigned long int>> P = search.getP();
    std::vector<unsigned long int> S = search.getS();
    std::vector<double> sigma = search.get_sigma();
    accumulation(P, S, sigma, s);

    //  rescaling
//    rescale(0);
}

void Betweenness::betweenness_centrality_of_i(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int i) {
    Bfs search;
    search.bfs(adj_matrix, i);
    std::unique_lock<std::mutex> lock (this->mutex);
    lock.unlock();
//  accumulation
    std::vector<std::vector<unsigned long int>> P = search.getP();
    std::vector<unsigned long int> S = search.getS();
    std::vector<double> sigma = search.get_sigma();
    accumulation(P, S, sigma, i);

    //  rescaling
//    rescale(0);
}

std::vector<double> Betweenness::betweenness_centrality() {
    return this->betweenness;
}

double Betweenness::mean_l() {
    return this->average_path;
}


void Betweenness::rescale(unsigned long int k) {
    unsigned long int N = this->betweenness.size();
    double scale = 0.5;
    if (k > 0) {
        scale = scale * static_cast<double>(N) / static_cast<double>(k);
    }

    for(unsigned long int v = 0; v < this->betweenness.size(); v++){
        this->betweenness[v] *= scale;
    }
}
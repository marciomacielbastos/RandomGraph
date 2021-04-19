#include <betweenness.h>

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
            std::unique_lock<std::mutex> lock (this->mutex);
            this->betweenness[w] += delta[w];
            lock.unlock();
        }
    }
}

void Betweenness::betweenness_centrality(Graph & G) {
    unsigned long int N = G.get_adj_matrix().size();
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < N; i++) {
        if ((i % n_threads) == 0) {
            betweenness_centrality_given_graph_and_source(G,i);
            while (!threads.empty()) {
                threads.back().join();
                threads.pop_back();
            }          
        }
        else {
            std::thread t (&Betweenness::betweenness_centrality_given_graph_and_source, this, std::ref(G), i);
            threads.push_back(std::move(t));
        }
    }
    while (!threads.empty()) {
        threads.back().join();
        threads.pop_back();
    }
    rescale(0);
    this->average_path /= N;
}


void Betweenness::betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix) {
    unsigned long int N = adj_matrix.size();
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < N; i++) {
        if ((i % n_threads) == 0) {
            betweenness_centrality_given_adj_matrix_and_source(adj_matrix,i);
            while (!threads.empty()) {
                threads.back().join();
                threads.pop_back();
            }
        }
        else {
            std::thread t (&Betweenness::betweenness_centrality_given_adj_matrix_and_source, this, std::ref(adj_matrix), i);
            threads.push_back(std::move(t));
        }
    }
    while (!threads.empty()) {
        threads.back().join();
        threads.pop_back();
    }
    rescale(0);
    this->average_path /= N;
}

void Betweenness::betweenness_centrality_given_graph_and_source(Graph & G, unsigned long int source) {
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    betweenness_centrality_given_adj_matrix_and_source(adj_matrix, source);

}

void Betweenness::betweenness_centrality_given_adj_matrix_and_source(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int source) {
    Breadth_first_search search;
    std::unique_lock<std::mutex> lock (this->mutex);
    search.search(adj_matrix, source);   
    lock.unlock();
//  accumulation
    std::vector<std::vector<unsigned long int>> parents_matrix = search.get_parent_matrix();
    std::vector<unsigned long int> spanning_tree = search.get_spanning_tree();
    std::vector<double> sigma = search.get_sigma();
    accumulation(parents_matrix, spanning_tree, sigma, source);

    //  rescaling
//    rescale(0);
}

std::vector<double> Betweenness::get_betweenness_centrality_vector() {
    return this->betweenness;
}

double Betweenness::mean_l() {
    return this->average_path;
}


void Betweenness::rescale(unsigned long int k) {
    double N = static_cast<double>(this->betweenness.size());
    double scale = 1.0 / ((N - 1) * (N - 2));
    if (k > 0) {
        scale = scale * static_cast<double>(N) / static_cast<double>(k);
    }

    for(unsigned long int v = 0; v < this->betweenness.size(); v++){
        this->betweenness[v] *= scale;
    }
}

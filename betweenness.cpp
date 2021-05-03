#include <betweenness.h>

Betweenness::Betweenness(const Betweenness &b1){
    this->betweenness = b1.betweenness;
    this->G = b1.G;
    this->n_threads = b1.n_threads;
}

Betweenness::Betweenness(Graph G): G(G) {
    this->n_threads = 1;
    create_betweenness_vector();
}

void Betweenness::create_betweenness_vector() {
    std::vector<double> b(this->G.get_N());
    this->betweenness = b;  // b[v]=0 for v in G
}

void Betweenness::set_num_of_threads(unsigned long n) {
    this->n_threads = n;
}

void Betweenness::flush_threads(std::vector<std::thread> &threads) {
    if (threads.size() > 0) {
        for (unsigned long int j = 0; j < threads.size(); j++) {
            threads[j].join();
        }
    }
    while (!threads.empty()) {
        threads.pop_back();
    }
}

void Betweenness::betweenness_centrality() {
    unsigned long int N = G.get_N();
    std::vector<std::thread> threads;
    create_betweenness_vector();
    for (unsigned long int i = 0; i < N; i++) {
        if ((i + 1) % n_threads) {
            std::thread t (&Betweenness::betweenness_centrality_from_source, this, i);
            threads.push_back(std::move(t));
        }
        else {
            betweenness_centrality_from_source(i);
            flush_threads(threads);
        }
    }
    flush_threads(threads);
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
            std::unique_lock<std::mutex> lock (std::ref(this->mutex));
            this->betweenness[w] += delta[w];
            lock.unlock();
        }
    }
}

void Betweenness::betweenness_centrality_given_adj_matrix_and_source(std::vector<std::vector<unsigned long int>> & adj_matrix, unsigned long int source) {
    Breadth_first_search search;
    std::unique_lock<std::mutex> lock (std::ref(this->mutex));
    search.search(adj_matrix, source);
    lock.unlock();
//  accumulation
    std::vector<std::vector<unsigned long int>> parents_matrix = search.get_parent_matrix();
    std::vector<unsigned long int> spanning_tree = search.get_spanning_tree();
    std::vector<double> sigma = search.get_sigma();
    accumulation(parents_matrix, spanning_tree, sigma, source);
}

void Betweenness::betweenness_centrality_from_source(unsigned long int source) {
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    betweenness_centrality_given_adj_matrix_and_source(adj_matrix, source);
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


void Betweenness::save (std::string filepath) {
    save(filepath, ",");
}

void Betweenness::save (std::string filepath, std::string separator) {
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < this->betweenness.size(); i++ ){
        if (i == this->betweenness.size() - 1) {
            myfile << this->betweenness[i];
        }
        else {
            myfile << this->betweenness[i] << separator;
        }
    }
    myfile.close();
}

std::vector<double> Betweenness::get_betweenness_centrality_vector() {
    return this->betweenness;
}

//void Betweenness::betweenness_centrality(std::vector<std::vector<unsigned long int>> & adj_matrix) {
//    unsigned long int N = adj_matrix.size();
//    std::vector<std::thread> threads;
//    for (unsigned long int i = 0; i < N; i++) {
//        if ((i % n_threads) == 0) {
//            betweenness_centrality_given_adj_matrix_and_source(adj_matrix,i);
//            while (!threads.empty()) {
//                threads.back().join();
//                threads.pop_back();
//            }
//        }
//        else {
//            std::thread t (&Betweenness::betweenness_centrality_given_adj_matrix_and_source, this, std::ref(adj_matrix), i);
//            threads.push_back(std::move(t));
//        }
//    }
//    while (!threads.empty()) {
//        threads.back().join();
//        threads.pop_back();
//    }
//    rescale(0);
//}

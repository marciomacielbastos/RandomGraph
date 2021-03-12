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


Betweenness::Betweenness(Graph &G, unsigned long int n_threads, unsigned long int sample_size, unsigned long int r) {
    this->n_threads = n_threads;
    this->average_path = 0;
    unsigned long int N = G.get_adj_matrix().size();
    std::vector<double> b(N);
    this->betweenness = b;  // b[v]=0 for v in G
    betweenness_centrality_approx(G, sample_size, r);
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

void Betweenness::accumulation_approx(unsigned long int d_size, std::vector<unsigned long int> &p, unsigned long int s, double r) {

    std::vector<double> delta(d_size);
    unsigned long int p_size = p.size();
    double mid;
    for (unsigned long int i = 0; i < p_size - 1; i++) {
        unsigned long int w = p[i];
        unsigned long int v = p[i + 1];
        delta[v] += (1 / r) + delta[w];
        //MUITO IMPORTANTE SABER O VALOR DE v!!!
        mid = std::floor(static_cast<double>((p_size - 1 - i)) / 2) - 1;
        v = static_cast<unsigned long int>(std::max(0.0, mid));
        v = p[p_size - v - 1];
        delta[v] -= (1 / r);
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

void Betweenness::betweenness_centrality_approx(Graph & G, unsigned long int sample_size, unsigned long int r) {
    unsigned long int N = G.get_adj_matrix().size();
    unsigned long int size;
    unsigned long int s;
    std::vector<std::thread> threads;
    std::vector<unsigned long int> sample = sample_pivots(N, sample_size);

    while (!sample.empty()) {
        size = std::min(this->n_threads - 1, sample.size() - 1);
        for (unsigned long int i = 0; i < size; i++) {
            s = sample.back();
            sample.pop_back();
            std::thread t (&Betweenness::betweenness_centrality_of_s_approx, this, std::ref(G), N, s, r);
            threads.push_back(std::move(t));
        }
        s = sample.back();
        sample.pop_back();
        betweenness_centrality_of_s_approx(G, N, s, r);
        while (!threads.empty()) {
            threads.back().join();
            threads.pop_back();
        }
    }
    this->average_path /= sample_size;
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

void Betweenness::betweenness_centrality_of_s_approx(Graph & G, unsigned long int N, unsigned long int s, unsigned long int r) {
    Bfs search;
    std::vector<std::vector<unsigned long int>> probabilities(N);
    search.bfs(G, s, probabilities);
    std::unique_lock<std::mutex> lock (this->mutex);
    this->average_path += search.get_l();
    lock.unlock();
//  accumulation
    std::vector<std::vector<unsigned long int>> P = search.getP();
    std::vector<unsigned long int> S = search.getS();
    unsigned long int d_size = P.size();
    for (int i = 0; i < static_cast<int>(r); i++) {
        std::vector<unsigned long int> path = build_path(probabilities, S, P);
        accumulation_approx(d_size, path, s, r);
    }


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


unsigned long int Betweenness::smart_pop(std::vector<unsigned long int> &list, unsigned long int idx) {
    unsigned long int temp = list[idx];
    list[idx] = list.back();
    list.pop_back();
    return temp;
}

std::vector<unsigned long int> Betweenness::sample_pivots(unsigned long N, unsigned long sample_size) {
    Uniform u;
    unsigned long int pop_size;
    unsigned long int idx;
    std::vector<unsigned long int> population(N);
    std::vector<unsigned long int> sample(sample_size);
    pop_size = population.size();
    for (unsigned long int i = 0; i < N; i++) {
        population[i] = i;
    }
    for (unsigned long i = 0; i < sample_size; i++) {
        idx = u.randint(pop_size);
        sample[i] = smart_pop(population, idx);
        pop_size = population.size();
    }
    return sample;
}

unsigned long int Betweenness::sample_parent(std::vector<std::vector<unsigned long> > probabilities,
                                             std::vector<std::vector<unsigned long> > P,
                                             unsigned long w) {
    Uniform u;
    if (P[w].size() > 1) {
        unsigned long int idx = u.randint(probabilities[w].back());
        idx = qsearch(idx, 0, probabilities[w].size() - 1, probabilities[w]);
        return P[w][idx];
    }
    else if (P[w].size() == 1) {
        return P[w][0];
    }
}

std::vector<unsigned long int> Betweenness::build_path(const std::vector<std::vector<unsigned long int>> &probabilities,
                                                       const std::vector<unsigned long int> &S,
                                                       const std::vector<std::vector<unsigned long int>> &P) {

    std::vector<unsigned long int> path;
    unsigned long int v;
    unsigned long int w = S.back();
    path.push_back(w);
    while (w != S[0]) {
        v = sample_parent(probabilities, P, w);
        path.push_back(v);
        w = v;
    }
    return path;
}

unsigned long int Betweenness::qsearch(unsigned long val, unsigned long l_idx, unsigned long u_idx, std::vector<unsigned long> list) {
    if (val <= list[l_idx]) {
        return l_idx;
    }
    else if (l_idx + 1 == u_idx) {
        return u_idx;
    }
    else {
        unsigned long int mid = static_cast<unsigned long int>(std::floor((static_cast<double>(u_idx) - static_cast<double>(l_idx)) / 2));
        mid += l_idx;
        if (val < list[mid]) {
            return qsearch(val, l_idx, mid, list);
        }
        else {
            return qsearch(val, mid, u_idx, list);
        }
    }
}

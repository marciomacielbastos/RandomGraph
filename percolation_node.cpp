#include "percolation_node.h"

Percolation_node::Percolation_node() : Percolation("degree", "binder"), lower(0), upper(1), num_pnts(0) {}

Percolation_node::Percolation_node(double lower, double upper, unsigned long int num_pnts) : Percolation("nodes", "nbinder"), lower(lower), upper(upper), num_pnts(num_pnts) {}

void Percolation_node::smart_pop(std::vector<unsigned long> &vect, unsigned long int idx) {
    vect[idx] = vect.back();
    vect.pop_back();
}

void Percolation_node::remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node) {
    for (unsigned long int idx = 0; idx < neighborhood.size(); ++idx) {
        if (neighborhood[idx] == node) {
           smart_pop(neighborhood, idx);
        }
    }
}

void Percolation_node::remove_node(std::vector<std::vector<unsigned long>> &adj_matrix, unsigned long int node) {
    unsigned long int neighbor;
    while (!adj_matrix[node].empty()) {
        neighbor = adj_matrix[node].back();
        adj_matrix[node].pop_back();
        remove_node_from_neighborhood(adj_matrix[neighbor], node);
    }
}

unsigned long int Percolation_node::calc_max_comp(unsigned long int N, std::vector<std::vector<unsigned long int>> &adj_matrix, unsigned long int ns) {
    UnionFind uf(N);
    unsigned long int v, w;
    for (unsigned long int i = 0; i < N; i++) {
        for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
            v = i;
            w = adj_matrix[i][j];
            if (!adj_matrix[w].empty()) {

                if (v < w) {
                    uf.union_(v, w);
                }
            }
        }
    }    
    return uf.get_size_of_max_comp();
}

double Percolation_node::stats_mean(std::vector<unsigned long> vect) {
    unsigned long int sum = 0;
    unsigned long int N = vect.size();
    double mean;
    for (auto value : vect) {
        sum += value;
    }
    if (N > 0) {
        mean = static_cast<double>(sum) / static_cast<double> (N);
    }
    else mean = -1;
    return mean;
}

double Percolation_node::stats_moment(std::vector<unsigned long> vect, unsigned int moment, double mean) {
    double moment_val;
    double sum = 0;
    double temp;
    unsigned long int N = vect.size();

    for (auto value : vect) {
        moment_val = static_cast<double>(value) - mean;
        temp = moment_val;
        for (unsigned int i=0; i < moment - 1; ++i) {
            temp *= moment_val;
        }
        sum += temp;
    }
    if (N > 0) {
        moment_val = sum / static_cast<double> (N);
    }
    return moment_val;
}

double Percolation_node::stats_moment(std::vector<unsigned long> vect, unsigned int moment) {
    double mean = stats_mean(vect);
    double moment_val = stats_moment(vect, moment, mean);
    return moment_val;
}

double Percolation_node::binder_cumulant(std::vector<unsigned long> vect, double mean) {
    double fourth_m = stats_moment(vect, 4, mean);
    double second_m = stats_moment(vect, 2, mean);
    double U = 1 - ((fourth_m) / (3 * second_m * second_m));
    return U;
}

double Percolation_node::binder_cumulant(std::vector<unsigned long> vect) {
    double mean = stats_mean(vect);
    double U = binder_cumulant(vect, mean);
    return U;
}

std::vector<double> Percolation_node::calc_check_points(unsigned long int N) {
    std::vector<double> chk_pnt;
    if (this->num_pnts == 0) this->num_pnts = N;
    unsigned long int num_pnts = this->num_pnts;
    num_pnts--;
    double step = (this->upper - this->lower) / static_cast<double>(num_pnts);
    for (long int i = num_pnts; i >= 0; i--) {
        chk_pnt.push_back(static_cast<double>(i) * step + this->lower);
    }
    return chk_pnt;
}

void Percolation_node::percolate(Graph & G) {
    Uniform u;
    unsigned long int number_of_nodes_removed = 0;
    unsigned long int vidx;
    unsigned long int v;
    unsigned long int N = G.get_n();
    unsigned long int max_n;
    unsigned long int neighbor;
    unsigned long int ns;
    double S_f;
    double progress = 0;
    double step;
    // Define check points
    std::vector<double> chk_pnt = calc_check_points(N);
    //Get the adjacent matrix
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    //Get list of nodes
    std::vector<unsigned long int> nodes(N,0);
    for (unsigned long int i=0; i < N; i++) {
        nodes[i] = i;
    }
    //fist point to record
    step = chk_pnt.back();
    chk_pnt.pop_back();
    while (!nodes.empty()) {
        if ((progress >= step) && (!chk_pnt.empty())) {
            // Calculate the size of the biggest component
            max_n = calc_max_comp(N, adj_matrix, ns);
            // Push the density of the biggest component
            S_f = static_cast<double>(max_n)/ static_cast<double>(N);
            this->result.push_back(S_f);
            step = chk_pnt.back();
            chk_pnt.pop_back();
        }
        vidx = u.randint(nodes.size());
        v = nodes[vidx];
        smart_pop(nodes,vidx);
        while (!adj_matrix[v].empty()) {
            neighbor = adj_matrix[v].back();
            adj_matrix[v].pop_back();
            remove_node_from_neighborhood(adj_matrix[neighbor], v);
        }
        number_of_nodes_removed++;
        progress = static_cast<double>(number_of_nodes_removed) / static_cast<double>(N);
    }
}

std::vector<double> Percolation_node::get_result() {
    return this->result;
}

std::vector<double> Percolation_node::get_other_result() {
    return this->other_result;
}

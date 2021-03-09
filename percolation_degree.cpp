#include "percolation_degree.h"

Percolation_degree::Percolation_degree() : Percolation("degree", "binder"), lower(0), upper(1), num_pnts(0) {}

Percolation_degree::Percolation_degree(double lower, double upper, unsigned long int num_pnts) : Percolation("degree", "binder"), lower(lower), upper(upper), num_pnts(num_pnts) {}

void Percolation_degree::smart_pop(std::vector<unsigned long> &vect, unsigned long int idx) {
    vect[idx] = vect.back();
    vect.pop_back();
}

void Percolation_degree::remove_node_from_neighborhood(std::vector<unsigned long> &neighborhood, unsigned long int node) {
    for (unsigned long int idx = 0; idx < neighborhood.size(); ++idx) {
        if (neighborhood[idx] == node) {
           smart_pop(neighborhood, idx);
        }
    }
}

void Percolation_degree::remove_node(std::vector<std::vector<unsigned long>> &adj_matrix, unsigned long int node) {
    unsigned long int neighbor;
    while (!adj_matrix[node].empty()) {
        neighbor = adj_matrix[node].back();
        adj_matrix[node].pop_back();
        remove_node_from_neighborhood(adj_matrix[neighbor], node);
    }
}

unsigned long int Percolation_degree::calc_max_comp(unsigned long int N, std::vector<std::vector<unsigned long int>> &adj_matrix) {
    UnionFind uf(N);
    for (unsigned long int i = 0; i < N; i++) {
        for (unsigned long int j = 0; j < adj_matrix[i].size(); j++) {
            if (!adj_matrix[j].empty()) {
                if (i < adj_matrix[i][j]) {
                    uf.union_(i, adj_matrix[i][j]);
                }
            }
        }
    }
    return uf.get_size_of_max_comp();
}

double Percolation_degree::stats_mean(std::vector<unsigned long> vect) {
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

double Percolation_degree::stats_moment(std::vector<unsigned long> vect, unsigned int moment, double mean) {
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

double Percolation_degree::stats_moment(std::vector<unsigned long> vect, unsigned int moment) {
    double mean = stats_mean(vect);
    double moment_val = stats_moment(vect, moment, mean);
    return moment_val;
}

double Percolation_degree::binder_cumulant(std::vector<unsigned long> vect, double mean) {
    double fourth_m = stats_moment(vect, 4, mean);
    double second_m = stats_moment(vect, 2, mean);
    double U = 1 - ((fourth_m) / (3 * second_m * second_m));
    return U;
}

double Percolation_degree::binder_cumulant(std::vector<unsigned long> vect) {
    double mean = stats_mean(vect);
    double U = binder_cumulant(vect, mean);
    return U;
}

std::vector<double> Percolation_degree::calc_check_points(unsigned long int N) {
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

void Percolation_degree::percolate(Graph & G) {
    unsigned long int number_of_nodes_removed = 0;
    unsigned long int N = G.get_n();
    unsigned long int root;
    unsigned long int neighbor;
    unsigned long int max_n;
    double progress = 0;
    double step;
    // Define check points
    std::vector<double> chk_pnt = calc_check_points(N);
    // Start a degree vector with N zeroes
    std::vector<unsigned long int> degrees (N, 0);
    //Get the adjacent matrix
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    // Update the degree vector with actual values
    for (unsigned long int i = 0; i < N; ++i) {
        degrees[i] = adj_matrix[i].size();
    }
    // Create heap using the degrees vector
    Heap_desc<unsigned long int> heap(degrees);
    step = chk_pnt.back();
    chk_pnt.pop_back();
    while (!heap.is_empty()) {
        if ((progress >= step) && (!chk_pnt.empty())) {
            step = chk_pnt.back();
            chk_pnt.pop_back();
            // Calculate the size of the biggest component
            max_n = calc_max_comp(N, adj_matrix);
            // Push the density of the biggest component
            this->result.push_back(static_cast<double>(max_n)/ static_cast<double>(N));
        }
        root = heap.pop();
        number_of_nodes_removed++;
        progress = static_cast<double>(number_of_nodes_removed) / static_cast<double>(N);
        while (!adj_matrix[root].empty()) {
            neighbor = adj_matrix[root].back();
            if (!adj_matrix[neighbor].empty()) {
                heap.update(neighbor);
            }
            adj_matrix[root].pop_back();
            remove_node_from_neighborhood(adj_matrix[neighbor], root);
        }
    }
    if (std::abs (step - 1.0) < (step / 10)) {
        // Calculate the size of the biggest component
        max_n = calc_max_comp(N, adj_matrix);
        // Push the density of the biggest component
        this->result.push_back(static_cast<double>(max_n)/ static_cast<double>(N));
    }
}

std::vector<double> Percolation_degree::get_result() {
    return this->result;
}

std::vector<double> Percolation_degree::get_other_result() {
    return this->other_result;
}

#include "breadth_first_search.h"

Breadth_first_search::Breadth_first_search() {

}

void Breadth_first_search::build_algorithm_vectors(unsigned long N){
    std::vector<double> sigma(N);
    this->sigma = sigma;
    std::vector<unsigned long int> S;
    this->spanning_tree = S;
    std::vector<std::vector<unsigned long int>> P(N);
    this->Parent_matrix = P;
}


void Breadth_first_search::search(Graph & G, unsigned long int source) {
    std::vector<std::vector<unsigned long int>> adj_matrix = G.get_adj_matrix();
    return search(adj_matrix, source);
}

// modified from Eppstein
void Breadth_first_search::search(std::vector<std::vector<unsigned long int>> adj_matrix, unsigned long int source) {
    unsigned long int N = adj_matrix.size();
    unsigned long int vertex;

    std::vector<unsigned long int> distance_from_source(N, N+1);
    std::queue<unsigned long int> queue_of_visited_vertices;
    double distance_sum = 0;
    double average_geodesical_path;

    build_algorithm_vectors(N);
    this->sigma[source] = 1.0;
    distance_from_source[source] = 0;
    queue_of_visited_vertices.push(source);

    while (!queue_of_visited_vertices.empty()) {
        vertex = queue_of_visited_vertices.front();
        this->spanning_tree.push_back(vertex);
        queue_of_visited_vertices.pop();
        for (auto w : adj_matrix[vertex]) {

            if (distance_from_source[w] > N) {
                distance_from_source[w] = distance_from_source[vertex] + 1;
                queue_of_visited_vertices.push(w);

                distance_sum += distance_from_source[w];
            }
            // If shortest path through w, count them all...
            if(distance_from_source[w] == distance_from_source[vertex] + 1) {
                this->sigma[w] += sigma[vertex];
                this->Parent_matrix[w].push_back(vertex);
            }
        }
    }
    average_geodesical_path = distance_sum/static_cast<double>(N - 1);
    this->average_geodesical_path;
}

double Breadth_first_search::get_average_geodesical_path() {
    return this->average_geodesical_path;
}

std::vector<std::vector<unsigned long int>> Breadth_first_search::get_parent_matrix() {
    return this->Parent_matrix;
}

std::vector<unsigned long int> Breadth_first_search::get_spanning_tree() {
    return this->spanning_tree;
}

std::vector<double> Breadth_first_search::get_sigma() {
    return this->sigma;
}

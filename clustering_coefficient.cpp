//#include "clustering_coefficient.h"

//Clustering_coefficient::Clustering_coefficient(Graph G) {
//    this->G = G;
//}

//void Clustering_coefficient::create_adjacent_matrix() {
//    unsigned long int N = this->G.get_N();
//    for (unsigned long int i = 0; i < (N - 1); i++) {
//        std::vector<bool> v(N - i - 1);
//        this->adjacent_matrix.push_back(v);
//    }
//}

//void Clustering_coefficient::fill_adjacent_matrix() {
//    unsigned long int first, second;
//    std::vector<std::pair<unsigned long int, unsigned long int>> links_vector = this->G.get_links_vector();
//    for (auto edge : links_vector) {
//        first = edge.first;
//        second = edge.second;
//        if (first < second) {
//            second = second - first - 1;
//            this->adjacent_matrix[first][second] = true;
//        }
//        else {
//            first = first - second - 1;
//            this->adjacent_matrix[second][first] = true;
//        }
//    }
//}

//void Clustering_coefficient::build_adjacent_matrix() {
//    create_adjacent_matrix();
//    fill_adjacent_matrix();
//}

//bool Clustering_coefficient::is_connected(unsigned long int v, unsigned long int w) {
//    if (v < w) {
//        return this->adjacent_matrix[v][w];
//    }
//    else if (v > w) {
//        return this->adjacent_matrix[w][v];
//    }
//    else return false;
//}

//double Clustering_coefficient::count_triangles(unsigned long int root) {
//    std::vector<unsigned long int> adj_vector = this->G.get_adj_matrix()[root];
//    unsigned long int n_size = adj_vector.size();
//    unsigned long int triangles = 0;
//    for (unsigned long int i = 0; i < n_size; i++) {
//        for (unsigned long int j = i + 1; j < n_size; j++) {
//            if (is_connected(adj_vector[i], adj_vector[j])) {
//                triangles++;
//            }
//        }
//    }
//    return static_cast<double>(triangles);
//}

//void Clustering_coefficient::clustering_of_vertex(unsigned long int i) {
//    double deg_vertice = static_cast<double>(this->G.get_adj_matrix()[i].size());
//    if (deg_vertice > 1) {
//        this->clustering_coefficient[i] = count_triangles(i) / (deg_vertice * (deg_vertice - 1));
//    }
//}

//std::vector<double> Clustering_coefficient::clustering() {
//    unsigned long int N = G.get_N();
//    create_clustering_coefficient_vector(N);
//    build_adjacent_matrix();
//    for (unsigned long int i = 0; i < N; ++i) {
//        clustering_of_vertex(i);
//    }
//    return clustering_coefficient;
//}

//void Clustering_coefficient::clustering_multithread(unsigned long int n_threads) {
//    std::vector<std::thread> threads;
//    unsigned long int N = G.get_N();
//    build_adjacent_matrix();
//    create_clustering_coefficient_vector(N);
//    for (unsigned int i = 0; i < N; i++) {
//        if ((i + 1) % n_threads) {
//            std::thread t (&Clustering_coefficient::clustering_of_vertex, this, i);
//            threads.push_back(std::move(t));
//        }
//        else {
//            clustering_of_vertex(i);
//            flush_threads(threads);
//        }
//    }
//    flush_threads(threads);
//}

//void Clustering_coefficient::create_clustering_coefficient_vector(unsigned long int N) {
//    std::vector<double> clustering_coefficient(N);
//    this->clustering_coefficient = clustering_coefficient;
//}

//void Clustering_coefficient::flush_threads(std::vector<std::thread> &threads) {
//    if (threads.size() > 0) {
//        for (unsigned long int j = 0; j < threads.size(); j++) {
//            threads[j].join();
//        }
//    }
//    while (!threads.empty()) {
//        threads.pop_back();
//    }
//}

//void Clustering_coefficient::save (std::string filepath) {
//    save(filepath, ",");
//}

//void Clustering_coefficient::save (std::string filepath, std::string separator) {
//    std::ofstream myfile;
//    myfile.open (filepath);
//    for(unsigned long int i = 0;  i < this->clustering_coefficient.size(); i++ ){
//        if (i == this->clustering_coefficient.size() - 1) {
//            myfile << this->clustering_coefficient[i];
//        }
//        else {
//            myfile << this->clustering_coefficient[i] << separator;
//        }
//    }
//    myfile.close();
//}

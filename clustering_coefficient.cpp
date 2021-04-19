//#include "clustering_coefficient.h"

//Clustering_coefficient::Clustering_coefficient() {

//}


//double Clustering_coefficient::mean(const std::vector<unsigned long int> &degrees) {
//    unsigned long int  N = degrees.size();
//    unsigned long int sum = 0;
//    for (unsigned long int i =0; i < N; i++) {
//        sum += degrees[i];
//    }
//    return static_cast<double>(sum) / static_cast<double>(N);
//}

//double Clustering_coefficient::square_mean(const std::vector<unsigned long int> &degrees) {
//    unsigned long int  N = degrees.size();
//    unsigned long int square_sum = 0;
//    for (unsigned long int i =0; i < N; i++) {
//        square_sum += (degrees[i] * degrees[i]);
//    }
//    return static_cast<double>(square_sum) / static_cast<double>(N);
//}

//double Clustering_coefficient::calculate_cluster_coefficient(Graph G) {
//    std::vector<unsigned long int> degrees = G.get_degree_distribution();
//    unsigned long int  N = degrees.size();
//    double k = mean(degrees);
//    double k2 = square_mean(degrees);
//    double num = (k2 - k) * (k2 - k);
//    double denom = static_cast<double>(N) * k * k * k;
//    return num / denom;
//}

//double Clustering_coefficient::calculate_cluster_coefficient(std::string filepath, unsigned long int N) {
//    Graph G(N);
//    G.read_file(filepath, ',');
//    return calculate_cluster_coefficient(G);
//}

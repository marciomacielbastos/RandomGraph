#include <QCoreApplication>
#include <iostream>
#include <string>
#include <sstream>
/* Show duration time of computation */
#include <chrono>
#include <time.h>

#include <betweenness.h>
#include <clustering_coefficient.h>
#include <percolation_vertex.h>
#include <percolation_degree.h>
#include <q_exp_graph_generator.h>


void progress_bar(unsigned long int i, double increment) {
    double progress = increment * static_cast<double>(i);
    unsigned int bar_width = 70;
    std::cout << "[";
    unsigned int pos = static_cast<unsigned int>(double(bar_width) * progress);
    for (unsigned int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<unsigned int>(progress * 100.0) << " %  \r";
    std::cout.flush();
    if(progress >= 1){
        std::cout << "\n";
    }
}

std::string get_filename(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned int i) {
    unsigned long int n = std::log2(N);
    std::string filename;
    std::stringstream ss;
    ss.str("");
    ss << gamma;
    ss << "_" << lambda;
    ss << "_" << kmin;
    ss << "_" << n;
    if (i < 10)  ss << "_00";
    else if (i < 100) ss << "_0";
    else ss << "_";
    ss << i;
    filename = ss.str() + ".txt";
    return filename;
}


int main(int argc, char** argv){
//    auto start_ini = std::chrono::high_resolution_clock::now();
//    double gamma = std::stod(argv[1]);
//    double lambda = std::stod(argv[1]);
//    unsigned long int  kmin = std::stoul(argv[3]);
//    unsigned long int  N = std::stoul(argv[2]);
//    double  l = std::stoul(argv[3]);
//    double  u = std::stoul(argv[4]);
//    std::string  filepath = argv[5];
//    topology_conf(gamma, lambda, kmin, N, filepath);
    /**********************************/
    /*            Parameters          */
    /**********************************/
    double gamma = 2.5;
    double lambda = 0;
    unsigned long int  kmin = 2;
    unsigned long int  N = 1024 * 4;
    unsigned long int num_rep = 100;
    unsigned long int n_threads = 5;
    std::string folder_simulated_graphs = "/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/";
    std::string folder_simulated_degrees = "/home/marcio/RandonGraph/Random-graph/output/degrees/";
    std::string folder_random_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/vertices/";
    std::string folder_mal_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/";
    std::string folder_clustering_coefficient = "/home/marcio/RandonGraph/Random-graph/output/clustering_coefficient/";
    std::string folder_betweenness = "/home/marcio/RandonGraph/Random-graph/output/betweenness/";
    std::string fname;

//    q_Exp_graph_generator q_gen = q_Exp_graph_generator(gamma,lambda,kmin,N);
//    q_gen.set_graph_folder(folder_simulated_graphs);
//    q_gen.set_degrees_folder(folder_simulated_degrees);
//    q_gen.generate_multiple_graphs(num_rep, n_threads);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout <<"Malicious Attack "<< "gamma: "<< gamma << " lambda: " << lambda << " kmin: "<< kmin << " N: " << N << ", " << num_rep << " times, on " << n_threads << " cores" << std::endl;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i);
        progress_bar(i, increment);
        Graph G (N);
        G.read_file(folder_simulated_graphs + fname, ',');
//        Percolation_degree p = Percolation_degree();
//        p.percolate_on_the_interval(G, 0.1, 0.2, 200);
//        p.save(folder_mal_attack + fname);
        Betweenness BC = Betweenness(G);
        BC.set_num_of_threads(n_threads);
        BC.betweenness_centrality();
        BC.save(folder_betweenness + fname);
//        Clustering_coefficient C = Clustering_coefficient(G);
//        C.clustering_multithread(n_threads);
//        C.save(folder_clustering_coefficient + fname);
    }
    progress_bar(num_rep, increment);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

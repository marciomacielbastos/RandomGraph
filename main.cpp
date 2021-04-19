#include <QCoreApplication>
#include <iostream>
#include <string>
#include <sstream>
/* Show duration time of computation */
#include <chrono>
#include <time.h>

#include <percolation_vertex.h>


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
//    double lambda = std::stod(argv[2]);
//    unsigned long int  kmin = std::stoul(argv[3]);
//    unsigned long int  N = std::stoul(argv[4]);
//    unsigned long int  num_rep = std::stoul(argv[5]);
//    std::string  filepath = argv[5];
//    topology_conf(gamma, lambda, kmin, N, filepath);
    /**********************************/
    /*            Parameters          */
    /**********************************/
    double gamma = 4;
    double lambda = 0.01;
    unsigned long int  kmin = 2;
    unsigned long int  N = 1024 * 64;
    unsigned long int num_rep = 100;
    unsigned long int n_threads = 1;
    std::string folder_simulated_graphs = "/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/";
    std::string folder_random_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/vertices/";
    std::string folder_mal_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/";
    std::string fname;

    auto start = std::chrono::high_resolution_clock::now();
    std::cout <<"Percolating "<< "gamma: "<< gamma << " lambda: " << lambda << " kmin: "<< kmin << " N: " << N << ", " << num_rep << " times, on " << n_threads << " cores" << std::endl;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i);
        progress_bar(i, increment);
        Graph G (N);
        Percolation_vertex p = Percolation_vertex();
        G.read_file(folder_simulated_graphs + fname, ',');
        p.percolate_on_the_interval(G, 0, 1, 100);
        p.save(folder_mal_attack + fname);
    }
    progress_bar(num_rep, increment);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

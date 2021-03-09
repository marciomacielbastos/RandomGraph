#include <QCoreApplication>
#include <iostream>
#include <cmath>
#include <uniform.h>
#include <q_exponential.h>
#include <qexponentialround.h>
#include <power_law.h>
#include <vector>
#include <list>
#include <deque>
#include <fstream>
#include <sstream>
#include <string>
#include <graph.h>
#include <heap_asc.h>
#include <topology_builder.h>
#include <topology_builder_configurational.h>
#include <percolation.h>
#include <percolation_degree.h>
#include <percolation_node.h>
#include <percolation_kcore.h>
#include <percolation_betweenness.h>
#include <percolation_edge.h>
#include <bitset>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <betweenness.h>
#include <statistical_calculus.h>
/* Show duration time of computation */
#include <chrono>
#include <time.h>


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


void write_degrees (std::vector<unsigned long int> degree_list, std::string path) {
    std::ofstream myfile;
    myfile.open (path);
    for(unsigned long int i = 0;  i < degree_list.size(); i++ ){
        if(i < degree_list.size() - 1) myfile << degree_list[i] << ",";
        else {
            myfile << degree_list[i] << std::endl;
        }
    }
    myfile.close();
}

void write_degrees (std::vector<unsigned long int> degree_list, double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned int i) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/degrees/outra/";
    std::string filename = get_filename(gamma, lambda, kmin, N, i);
    std::ofstream myfile;
    std::string path = folder + filename + ".txt";
    myfile.open (path);
    for(unsigned long int i = 0;  i < degree_list.size(); i++ ){
        if(i < degree_list.size() - 1) myfile << degree_list[i] << ",";
        else {
            myfile << degree_list[i] << std::endl;
        }
    }
    myfile.close();
}

void write_graph (std::vector<std::pair<long unsigned int, long unsigned int> > link_list, std::string filepath) {
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < link_list.size(); i++ ){
        myfile << link_list[i].first << " " << link_list[i].second << std::endl;
    }
    myfile.close();
}

void write_graph (std::vector<std::pair<long unsigned int, long unsigned int>> link_list, double gamma, double lambda, int kmin, unsigned long int N, unsigned long int i) {
    std::string folder ("/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/");
    std::string filename =  get_filename(gamma, lambda, kmin, N, i);
    std::ofstream myfile;
    std::string path = folder + filename;
    myfile.open (path);
    for(i = 0;  i < link_list.size(); i++ ){
        myfile << link_list[i].first << "," << link_list[i].second << std::endl;
    }
    myfile.close();
}

void write_realization (std::string path, std::vector<double> &realization) {
    std::ofstream myfile;
    myfile.open (path);
    for(unsigned long int i = 0;  i < realization.size(); i++ ){
//        myfile << other_result[i] << "," << other_result[i][1]<< std::endl;
        if (i == realization.size() - 1) {
            myfile << realization[i];
        }
        else {
            myfile << realization[i] << ",";
        }
    }
    myfile.close();
}

std::vector<unsigned long int> dist_calc(Distribution *dist, unsigned long int N) {
    std::vector<unsigned long int> degree_list;
    while (N > 0){
        unsigned long int val = dist->randint();
        degree_list.push_back(val);
        N--;
    }
    return degree_list;
}

std::vector<unsigned long int> calc_degree_list(double gamma, double lambda, unsigned long int kmin, unsigned long int N){   
    std::vector<unsigned long int> degree_list;
    Distribution *dist;
    if (lambda > 0) {
        double q = (gamma + 1) / gamma;
//        dist = new qExponential(lambda, q, kmin, N);
        dist = new qExponentialRound(lambda, q, kmin, N);
    }
    else {
        dist = new Power_law(gamma, kmin, N);
    }

    degree_list = dist_calc(dist, N);
    return degree_list;
}

Graph calc_topology_with_configurational_method(std::vector<unsigned long int> degree_list){
    TopologyBuilderConfigurational tb = TopologyBuilderConfigurational(degree_list);
    tb.random_link();
    Graph G = tb.get_g();
    return G;
}

unsigned long int get_max(std::vector<unsigned long int> degs) {
    unsigned long int size = degs.size();
    unsigned long int val = 0;
    for (unsigned long int i=0; i < size; i++) {
        if (val < degs[i]) {
            val = degs[i];
        }
    }
    return val;
}

void thread_topology_conf(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned long int i) {
    std::string folder_degrees = "/home/marcio/RandonGraph/Random-graph/output/degrees/";
    std::string fname = get_filename(gamma, lambda, kmin, N, i);
    std::vector<unsigned long int> degree_list = calc_degree_list(gamma, lambda, kmin, N);
    Graph G = calc_topology_with_configurational_method(degree_list);
    std::vector<std::pair<long unsigned int, long unsigned int>> link_list = G.get_link_list();
    degree_list = G.get_degree_distribution();
    write_degrees(degree_list, folder_degrees + fname);
    write_graph(link_list, gamma, lambda, kmin, N, i);
}

void topology_conf(double gamma, double lambda, unsigned long int kmin, unsigned long int N, std::string filepath) {
    std::vector<unsigned long int> degree_list = calc_degree_list(gamma, lambda, kmin, N);
    Graph G = calc_topology_with_configurational_method(degree_list);
    std::vector<std::pair<long unsigned int, long unsigned int>> link_list = G.get_link_list();
//    degree_list = G.get_degree_distribution();
//    write_degrees(degree_list, filepath);
    write_graph(link_list, filepath);
}

void simulation(unsigned long int repetitions, unsigned long int n_threads, double gamma, double lambda, unsigned long int kmin, unsigned long int N) {
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < repetitions; i++) {
        if ((i + 1) % n_threads) {
            std::thread t (&thread_topology_conf, gamma, lambda, kmin, N, i);
            threads.push_back(std::move(t));
        }
        else {
            thread_topology_conf(gamma, lambda, kmin, N, i);
            if (threads.size() > 0) {
                for (unsigned long int j = 0; j < n_threads - 1; j++) {
                    threads[j].join();
                }
            }
            while (!threads.empty()) {
                threads.pop_back();
            }
        }

     }
    if (threads.size() > 0) {
        for (int j = 0; j < threads.size(); j++) {
            threads[j].join();
        }
    }
    while (!threads.empty()) {
        threads.pop_back();
    }
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
    double gamma = 2.5;
    double lambda = 10;
    unsigned long int  kmin = 2;
    unsigned long int  N = 32768 / 8;
    unsigned long int num_rep = 20;
    unsigned long int n_threads = 4;
    std::string folder_degrees = "/home/marcio/RandonGraph/Random-graph/output/degrees/";
    std::string folder_simulated_graphs = "/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/";
    std::string folder_betweeness = "/home/marcio/RandonGraph/Random-graph/output/betweenness/";
    std::string fname;

    std::cout <<"Simulating "<< "gamma: "<< gamma << " lambda: " << lambda << " kmin: "<< kmin << " N: " << N << ", " << num_rep << " times, on " << n_threads << " cores" << std::endl;
    simulation(num_rep, n_threads, gamma, lambda, kmin, N);
    std::cout <<"Simulation done!"<< std::endl;
    std::vector<double> betweenness;
    auto start = std::chrono::high_resolution_clock::now();
    Statistical_calculus sc;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i);
        sc.progress_bar(i, increment);
        Graph G (N);
        G.read_file(folder_simulated_graphs + fname, ',');
        Betweenness b(G, n_threads);
        betweenness = b.betweenness_centrality();
        write_realization(folder_betweeness + fname, betweenness);
    }
    sc.progress_bar(num_rep, increment);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;



//    auto start = std::chrono::high_resolution_clock::now();
//    Statistical_calculus sc;
//    std::cout << gamma << " " << N << std::endl;

//    Percolation *p = new Percolation_node(0, 1, 100);
//    sc.add_percolation(p);
//    double increment = 1 / static_cast<double>(num_rep);
//    num_rep += 300;
//    for (unsigned long int i = 300; i < num_rep; i++) {
//        fname = get_filename(gamma, lambda, kmin, N, i);
//        sc.progress_bar(i, increment);
//        Graph G (N);
//        Percolation_node p (0, 1, 101);
//        G.read_file(folder_simulated_graphs + fname, ',');
//        p.percolate(G);
//        std::vector<double> result = p.get_result();
//        write_realization(folder_random_attack + fname, result);
//    }
//    sc.progress_bar(700, increment);
//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

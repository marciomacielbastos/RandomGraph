#include <QCoreApplication>
#include <iostream>
#include <uniform.h>
#include <q_exponential.h>
#include <vector>
#include <list>
#include <deque>
#include <fstream>
#include <sstream>
#include <graph.h>
#include <heap_asc.h>
#include <topology_builder.h>
#include <percolation.h>
#include <percolation_degree.h>
#include <bitset>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <betweenness.h>
/* Show duration time of computation */
#include <chrono>
#include <time.h>
#include <statistical_calculus.h>

void write_degrees (std::vector<unsigned long int> degree_list, int i, unsigned long int N, int kmin) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/degrees/";
    unsigned long int n = std::log2(N);
    std::string out_string;
    std::stringstream ss;
    ss << i;
    ss << "_" << kmin;
    ss << "_" << n;
    out_string = ss.str();
    std::ofstream myfile;
    std::string filename = folder + out_string + "_degrees.txt";
    myfile.open (filename);
    for(unsigned long int i = 0;  i < degree_list.size(); i++ ){
        if(i < degree_list.size() - 1) myfile << degree_list[i] << ",";
        else {
            myfile << degree_list[i] << std::endl;
        }
    }
    myfile.close();
}

void write_graph (std::vector<std::pair<long unsigned int, long unsigned int> > link_list, double gamma, double lambda, int kmin, unsigned long int N, int i) {
    std::string folder ("/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/");
    unsigned long int n = std::log2(N);
    std::string out_string;
    std::stringstream ss;
    ss << gamma;
    ss << "_" << lambda;
    ss << "_" << kmin;
    ss << "_" << n;
    ss << "_" << i;
    out_string = ss.str();
    std::ofstream myfile;
    std::string filename = folder + out_string + ".txt";
    myfile.open (filename);
    for(unsigned long int i = 0;  i < link_list.size(); i++ ){
        myfile << link_list[i].first << "," << link_list[i].second << std::endl;
    }
    myfile.close();
}

void write_results (std::string folder, std::vector<std::vector<double>> &result, int i, unsigned long int N, int kmin) {
    unsigned long int n = std::log2(N);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "_" << i;
    ss << "_" << kmin;
    out_string = ss.str();
    std::ofstream myfile;
    std::string filename = folder + out_string + ".txt";
    myfile.open (filename);
    if (result[0].size() < 4) {
        for(unsigned long int i = 0;  i < result.size(); i++ ){
            myfile << result[i][0] << "," << result[i][1]<< std::endl;
        }
    }
    else {
        for(unsigned long int i = 0;  i < result.size(); i++ ){
            myfile << result[i][0] << "," << result[i][1] << "," << result[i][2] << "," << result[i][3]<< std::endl;
        }
    }
    myfile.close();
}

//void calc_config (unsigned int num_rep, int i, unsigned long int N, double gamma, double lambda, int kmin, unsigned long int noc, unsigned long int n_threads) {
////    std::string folder_1 = "/home/marcio/RandonGraph/Random-graph/output/percolation/betweeness/";
//    std::string folder_2 = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/";
//    std::string folder_3 = "/home/marcio/RandonGraph/Random-graph/output/percolation/kcore/";
//    std::string folder_4 = "/home/marcio/RandonGraph/Random-graph/output/percolation/edge/";
//    std::string folder_5 = "/home/marcio/RandonGraph/Random-graph/output/meanl/";
//    std::vector<std::vector<double>> result;
//    noc = noc - 2;
//    double q = (gamma + 1) / gamma;
//    unsigned long int n = std::log2(N);
//    std::string out_string;
//    std::stringstream ss;
//    ss << n;
//    ss << "_" << i;
//    ss << "_" << kmin;
//    std::cout <<"N: 2^"<<n<<", (q="<< q <<", lambda="<< lambda << "), kmin: "<<kmin<< std::endl;
//    out_string = ss.str();
//    qExponential q_exp = qExponential(lambda, q, kmin, N - 1);
//    Percolation p(&q_exp, N, noc);
//    p.percolation_configurational(num_rep, n_threads);
////    result = p.get_betweeness_result();
////    write_results(folder_1, result, i, N, kmin);
//    result = p.get_degree_result();
//    write_results(folder_2, result, i, N, kmin);
//    result = p.get_kcore_result();
//    write_results(folder_3, result, i, N, kmin);
//    result = p.get_edge_result();
//    write_results(folder_4, result, i, N, kmin);
//        q = p.get_mean_l();
//        write_mean_l(folder_5, q, i, N, kmin);
//}

void temp (std::vector<double> result, double gamma, double lambda, unsigned long int kmin, unsigned long int n) {
    std::string path = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/";
    std::stringstream filename;
    filename.str("");
    filename << path;
    filename << gamma;
    filename << "_" << lambda;
    filename << "_" << kmin;
    filename << "_" << n;
    filename << ".txt";
    std::ofstream myfile;
    path = filename.str();
    myfile.open(path);
    for(unsigned long int i = 0;  i < result.size(); i++ ){
        myfile << result[i] << std::endl;
    }
    myfile.close();
}


std::vector<unsigned long int> calc_degree_list(double gamma, double lambda, unsigned long int kmin, unsigned long int N){
    double q = (gamma + 1) / gamma;
    qExponential q_exp = qExponential(lambda, q, kmin, N - 1);
    std::vector<unsigned long int> degree_list;
    unsigned long int j = 0;
    while (j < N){
        unsigned long int val = q_exp.randint();
        if (val > 1){
            degree_list.push_back(val);
            j++;
        }
    }
    return degree_list;
}

Graph calc_topology_with_configurational_method(std::vector<unsigned long int> degree_list){
    TopologyBuilderConfigurational tb = TopologyBuilderConfigurational(degree_list);
    tb.random_link();
    Graph G = tb.get_g();
    return G;
}

void thread_topology_conf(double gamma, double lambda, unsigned long int kmin, unsigned long int N, int i) {
    std::vector<unsigned long int> degree_list = calc_degree_list(gamma, lambda, kmin, N);
    Graph G = calc_topology_with_configurational_method(degree_list);
    std::vector<std::pair<long unsigned int, long unsigned int>> link_list = G.get_link_list();
    write_graph(link_list, gamma, lambda, kmin, N, i);
}

void simulation(int repetitions, int n_threads, double gamma, double lambda, unsigned long int kmin, unsigned long int N) {
    std::vector<std::thread> threads;
    for (unsigned long int i = 0; i < repetitions; i++) {
        if ((i + 1) % n_threads) {
            std::thread t (&thread_topology_conf, gamma, lambda, kmin, N, i);
            threads.push_back(std::move(t));
        }
        else {
            thread_topology_conf(gamma, lambda, kmin, N, i);
            if (threads.size() > 0) {
                for (int j = 0; j < n_threads - 1; j++) {
                    threads[j].join();
                }
            }
            while (!threads.empty()) {
                threads.pop_back();
            }
        }
     }
    if (threads.size() > 0) {
        for (int j = 0; j < n_threads; j++) {
            threads[j].join();
        }
    }
    while (!threads.empty()) {
        threads.pop_back();
    }
}

int main(int argc, char *argv[]){
    auto start_ini = std::chrono::high_resolution_clock::now();

    /**********************************/
    /*            Parameters          */
    /**********************************/
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    double lambda_values[5] = {1.751, 0.551, 0.334, 0.26, 0.223};
    double lambda = 1;
    unsigned long int N[10] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
    unsigned long int num_rep[10] = {120, 110, 100, 90, 80, 70, 60, 50, 40, 30};
    unsigned long int kmin = 2;
    unsigned long int noc = N[0];
    unsigned long int n;
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/";
    std::stringstream path;
    for(int i = 0; i < 10; i++){
        for (int j = 0; j < 5; j++) {
            std::cout << gamma_values[j] << " " << N[i] << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            Statistical_calculus sc;
            Percolation *p = new Percolation_degree;
            sc.add_percolation(p);
            sc.calc(gamma_values[j], lambda, kmin, N[i], num_rep[i], folder);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
        }
    }

//    for(int i = 0; i < 10; i++){
//        for (int j = 0; j < 5; j++) {

//            auto start = std::chrono::high_resolution_clock::now();
//            std::cout <<"start (f: "<< N[i] <<" | i: "<< j << ")" << std::endl;
//            simulation(num_rep[i], 5, gamma_values[j], 1, kmin, N[i]);
//            std::cout <<"Simulation done!"<< std::endl;
//            auto stop = std::chrono::high_resolution_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//            std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
//        }
//    }


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_ini);
    std::cout << "Done! Total duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

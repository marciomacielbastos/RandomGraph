#include <QCoreApplication>
#include <iostream>
#include <uniform.h>
#include <q_exponential.h>
#include <power_law.h>
#include <vector>
#include <list>
#include <deque>
#include <fstream>
#include <sstream>
#include <graph.h>
#include <heap_asc.h>
#include <topology_builder.h>
#include <topology_builder_configurational.h>
#include <percolation.h>
#include <percolation_degree.h>
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
    std::string folder ("/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/");
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

void write_results (std::string folder, std::vector<std::vector<double>> &result, double gamma, double lambda, unsigned int kmin, unsigned long int N) {
    unsigned long int n = std::log2(N);
    std::string path;
    std::stringstream filename;
    filename.str("");
    filename << gamma;
    filename << "_" << lambda;
    filename << "_" << kmin;
    filename << "_" << n;
    std::ofstream myfile;
    path = folder + filename.str() + ".txt";
    myfile.open (path);
    for(unsigned long int i = 0;  i < result.size(); i++ ){
        myfile << result[i][0] << "," << result[i][1]<< std::endl;
    }
    myfile.close();
}

void write_results (std::string folder, std::vector<double> &other_result, double gamma, double lambda, unsigned int kmin, unsigned long int N) {
    unsigned long int n = std::log2(N);
    std::string path;
    std::stringstream filename;
    filename.str("");
    filename << gamma;
    filename << "_" << lambda;
    filename << "_" << kmin;
    filename << "_" << n;
    std::ofstream myfile;
    path = folder + filename.str() + ".txt";
    myfile.open (path);
    for(unsigned long int i = 0;  i < other_result.size(); i++ ){
        if (i == other_result.size() - 1) {
            myfile << other_result[i];
        }
        else {
            myfile << other_result[i] << ",";
        }
    }
    myfile.close();
}


std::vector<unsigned long int> calc_degree_list(double gamma, double lambda, unsigned long int kmin, unsigned long int N){
    double q = (gamma + 1) / gamma;
    qExponential dist = qExponential(lambda, q, kmin, N - 1);
//    Power_law dist = Power_law(gamma, kmin, N - 1);
    std::vector<unsigned long int> degree_list;
    unsigned long int j = 0;
    while (j < N){
        unsigned long int val = dist.randint();
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
    for (int i = 0; i < repetitions; i++) {
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

int main(){
    auto start_ini = std::chrono::high_resolution_clock::now();

    /**********************************/
    /*            Parameters          */
    /**********************************/
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    unsigned long int N[10] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
    unsigned long int num_rep[10] = {120, 110, 100, 90, 80, 70, 60, 50, 40, 30};
    unsigned long int kmin = 2;  
    double lambda_values[2] = {0.1, 0.01};
    for (int k = 0; k < 2; ++k) {
        if (k > 0) {
            std::cout << "lambda: " << lambda_values[k] <<std::endl;
            for (int i = 5; i < 10; i++) {
                for (int j = 0; j < 4; j++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    std::cout <<"start (f: "<< N[i] <<" | gamma: "<< gamma_values[j] << ")" << std::endl;
                    simulation(num_rep[i], 5, gamma_values[j], lambda_values[k], kmin, N[i]);
                    std::cout <<"Simulation done!"<< std::endl;
                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
                }
            }
        }
        std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
        std::string subfolder_1 = "simulated_graphs/q_exponential/";
        std::string subfolder_2 = "percolation/";
        std::string path;
        for (int i = 5; i < 10; i++){
            for (int j = 0; j < 4; j++) {
                path = folder + subfolder_1;
                std::cout << gamma_values[j] << " " << N[i] << std::endl;
                auto start = std::chrono::high_resolution_clock::now();
                Statistical_calculus sc;
                Percolation *p = new Percolation_degree;
                sc.add_percolation(p);
    //            p = new Percolation_degree;
    //            sc.add_percolation(p);
                sc.calc(gamma_values[j], lambda_values[k], kmin, N[i], num_rep[i], path);
                std::vector<std::vector<std::vector<double>>> results = sc.get_results();
                std::vector<std::vector<double>> other_results = sc.get_other_results();
                for (unsigned int k=0; k < results.size(); k++) {
                    path = folder + subfolder_2 + sc.get_percolation_name(k) + "/";
                    write_results(path, results[k], gamma_values[j], lambda_values[k], kmin, N[i]);
                    if (!other_results[k].empty()) {
                        path = folder + subfolder_2 + sc.get_percolation_other(k) + "/";
                        write_results(path, other_results[k], gamma_values[j], lambda_values[k], kmin, N[i]);
                    }
                }
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_ini);
    std::cout << "Done! Total duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

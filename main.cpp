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
#include <topology_builder.h>
#include <percolation.h>
#include <bitset>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

/* Show duration time of computation */
#include <chrono>
#include <time.h>



void write_degrees (std::vector<unsigned long int> degree_list, int i, unsigned long int f, int kmin) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/degrees/";
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E3";
    ss << "_" << i;
    ss << "_" << kmin;
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

void write_links (std::vector<std::pair<long unsigned int, long unsigned int> > link_list, int i, unsigned long int f, int kmin) {
    std::string folder ("/home/marcio/RandonGraph/Random-graph/output/links/");
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E3";
    ss << "_" << i;
    ss << "_" << kmin;
    out_string = ss.str();
    std::ofstream myfile;
    std::string filename = folder + out_string + "_links.txt";
    myfile.open (filename);
    for(unsigned long int i = 0;  i < link_list.size(); i++ ){
        myfile << link_list[i].first << "," << link_list[i].second << std::endl;
    }
    myfile.close();
}

void calc (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E3";
    ss << "_" << i;
    ss << "_" << kmin;
    std::cout <<"N: 2^"<<n<<"E3"<<", (q="<< q <<", lambda="<< lambda << "), kmin: "<<kmin<< std::endl;
    out_string = ss.str();
    qExponential q_exp = qExponential(lambda, q, kmin, N - 1);
    Percolation p(&q_exp, N);
    p.percolation_molloy_reed(num_rep);
    p.write_percolation_results(folder + out_string + ".txt");
}

void calc_config (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E3";
    ss << "_" << i;
    ss << "_" << kmin;
    std::cout <<"N: 2^"<<n<<"E3"<<", (q="<< q <<", lambda="<< lambda << "), kmin: "<<kmin<< std::endl;
    out_string = ss.str();
    qExponential q_exp = qExponential(lambda, q, kmin, N - 1);
    Percolation p(&q_exp, N);
    p.percolation_configurational(num_rep);
    p.write_percolation_results(folder + out_string + ".txt");
}

void calc_t (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin, int thread_id) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E3";
    ss << "_" << i;
    ss << "_" << kmin;
    out_string = ss.str();
    qExponential q_exp = qExponential(lambda, q, 1, N - 1);
    Percolation p(&q_exp, N, thread_id);
    p.percolation_molloy_reed(num_rep);
    p.write_percolation_results(folder + out_string + ".txt");

}

std::vector<unsigned long int> test_degree_list(int i, unsigned long int f, double gamma, double lambda, int kmin){
    unsigned long int N = static_cast<unsigned long int>(1E3);
    N *= f;
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
    write_degrees(degree_list, i, f, kmin);
    return degree_list;
}

Graph test_topology(std::vector<unsigned long int> degree_list){
    TopologyBuilder tb = TopologyBuilder(degree_list);
    return tb.get_g();
}

std::vector<std::pair<long unsigned int, long unsigned int> > test_topology_conf(std::vector<unsigned long int> degree_list, int i, unsigned long int f, int kmin){
    TopologyBuilderConfigurational tb = TopologyBuilderConfigurational(degree_list);
    tb.random_link();
    std::vector<std::pair<long unsigned int, long unsigned int>> link_list = tb.get_g().get_link_list();
    write_links(link_list, i, f, kmin);
    return link_list;
}

int main(int argc, char *argv[]){
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long int f[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    /**********************************/
    /*            Parameters          */
    /**********************************/

    unsigned int num_rep = 100;
    int kmin = 2;
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    double lambda_values[5] = {1.751, 0.551, 0.334, 0.26, 0.223};

    for(int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++) {
            std::cout <<"start (f: "<< f[j] <<" | i: "<< i << ")" << std::endl;
            std::vector<unsigned long int> degree_list = test_degree_list(i, f[j], gamma_values[i], lambda_values[0], kmin);
            std::cout <<"test_degree_list done!"<< std::endl;
            test_topology_conf(degree_list, i, f[j], kmin);
            std::cout <<"test_topology_conf done!"<< std::endl;
            calc_config(num_rep, i, f[j], gamma_values[i], lambda_values[0], kmin);
            std::cout <<"calc_config done!"<< std::endl;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

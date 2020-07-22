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


void calc (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
    unsigned long int N = static_cast<unsigned long int>(1E4);
    N *= f;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E4";
    ss << "_" << i;
    ss << "_" << kmin;
    std::cout <<"N: 2^"<<n<<"E4"<<", (q="<< q <<", lambda="<< lambda << "), kmin: "<<kmin<< std::endl;
    out_string = ss.str();
    q_Exponential q_exp = q_Exponential(lambda, q, kmin, N - 1);
    Percolation p(&q_exp, N);
    p.percolation_molloy_reed(num_rep);
    p.write_random_vector(folder + out_string + ".txt");

}

void calc_t (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin, int thread_id) {
    std::string folder = "/home/marcio/RandonGraph/Random-graph/output/";
    unsigned long int N = static_cast<unsigned long int>(1E4);
    N *= f;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(f);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "E4";
    ss << "_" << i;
    ss << "_" << kmin;
    out_string = ss.str();
    q_Exponential q_exp = q_Exponential(lambda, q, 1, N - 1);
    Percolation p(&q_exp, N, thread_id);
    p.percolation_molloy_reed(num_rep);
    p.write_random_vector(folder + out_string + ".txt");

}

std::vector<unsigned long int> test_degree_list(unsigned long int f, double gamma, double lambda, int kmin){
    unsigned long int N = static_cast<unsigned long int>(1E4);
    N *= f;
    double q = (gamma + 1) / gamma;
    q_Exponential q_exp = q_Exponential(lambda, q, kmin, N - 1);
    std::vector<unsigned long int> degree_list;
    for(unsigned long int j = 0; j < N; j++){
        unsigned long int val = q_exp.randint();
        degree_list.push_back(val);
    }
    return degree_list;
}

Graph test_topology(std::vector<unsigned long int> degree_list){
    Topology_builder tb = Topology_builder(degree_list);
    return tb.get_g();
}

int main(int argc, char *argv[]){
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long int f[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    /**********************************/
    /*            Parameters          */
    /**********************************/

    unsigned int num_rep = 100;
    int kmin = 1;
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    double lambda_values[5] = {1.751, 0.551, 0.334, 0.26, 0.223};
    int i = 3;
    int j = 5;
    std::vector<unsigned long int> degree_list = test_degree_list(f[j], gamma_values[i], lambda_values[i], kmin);
//    for (int j = 0; j < 8; j++) {
//        calc(num_rep, i, f[j], gamma_values[i], lambda_values[i], kmin);
//    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "test_degree_list duration: (" << duration.count() << " millisecond)"<< std::endl;

    test_topology(degree_list);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "test_topology duration: (" << duration.count() << " millisecond)"<< std::endl;

}

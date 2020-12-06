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
#include <bitset>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <betweenness.h>
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

void write_links (std::vector<std::pair<long unsigned int, long unsigned int> > link_list, int i, unsigned long int N, int kmin) {
    std::string folder ("/home/marcio/RandonGraph/Random-graph/output/links/");
    unsigned long int n = std::log2(N);
    std::string out_string;
    std::stringstream ss;
    ss << i;
    ss << "_" << kmin;
    ss << "_" << n;
    out_string = ss.str();
    std::ofstream myfile;
    std::string filename = folder + out_string + "_links.txt";
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

void write_mean_l (std::string folder, double mean_l, int i, unsigned long int N, int kmin) {
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
    myfile << mean_l << std::endl;
    myfile.close();
}

void calc_config (unsigned int num_rep, int i, unsigned long int N, double gamma, double lambda, int kmin, unsigned long int noc, unsigned long int n_threads) {
    std::string folder_1 = "/home/marcio/RandonGraph/Random-graph/output/percolation/betweeness/";
    std::string folder_2 = "/home/marcio/RandonGraph/Random-graph/output/percolation/kcore/";
    std::string folder_3 = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/";
    std::string folder_4 = "/home/marcio/RandonGraph/Random-graph/output/percolation/edge/";
    std::string folder_5 = "/home/marcio/RandonGraph/Random-graph/output/meanl/";
    std::vector<std::vector<double>> result;
    noc = noc - 2;
    double q = (gamma + 1) / gamma;
    unsigned long int n = std::log2(N);
    std::string out_string;
    std::stringstream ss;
    ss << n;
    ss << "_" << i;
    ss << "_" << kmin;
    std::cout <<"N: 2^"<<n<<", (q="<< q <<", lambda="<< lambda << "), kmin: "<<kmin<< std::endl;
    out_string = ss.str();
    qExponential q_exp = qExponential(lambda, q, kmin, N - 1);
    Percolation p(&q_exp, N, noc);
    p.percolation_configurational(num_rep, n_threads);
    result = p.get_betweeness_result();
    write_results(folder_1, result, i, N, kmin);
    result = p.get_degree_result();
    write_results(folder_2, result, i, N, kmin);
    result = p.get_kcore_result();
    write_results(folder_3, result, i, N, kmin);
    result = p.get_edge_result();
    write_results(folder_4, result, i, N, kmin);
    q = p.get_mean_l();
    write_mean_l(folder_5, q, i, N, kmin);
}

std::vector<unsigned long int> test_degree_list(int i, unsigned long int N, double gamma, double lambda, int kmin){
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
    write_degrees(degree_list, i, N, kmin);
    return degree_list;
}

Graph test_topology(std::vector<unsigned long int> degree_list){
    TopologyBuilder tb = TopologyBuilder(degree_list);
    return tb.get_g();
}

//std::vector<std::pair<long unsigned int, long unsigned int> >
void test_topology_conf(std::vector<unsigned long int> degree_list, int i, unsigned long int N, int kmin){
    TopologyBuilderConfigurational tb = TopologyBuilderConfigurational(degree_list);
    tb.random_link();
    Graph G = tb.get_g();
//    Betweenness btwnss(G, 5);
    std::vector<std::pair<long unsigned int, long unsigned int>> link_list = tb.get_g().get_link_list();
    write_links(link_list, i, N, kmin);
}

int main(int argc, char *argv[]){
    auto start_ini = std::chrono::high_resolution_clock::now();
//    unsigned long int N[10] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
    unsigned long int N[6] = {1024, 2048, 4096, 8192, 16384, 32768};
    /**********************************/
    /*            Parameters          */
    /**********************************/

    unsigned int num_rep[5] = {70, 60, 50, 40, 30};
    int kmin = 2;
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    double lambda_values[5] = {1.751, 0.551, 0.334, 0.26, 0.223};
    unsigned long int noc = N[0];
    for(int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++) {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout <<"start (f: "<< N[j] <<" | i: "<< i << ")" << std::endl;
            std::vector<unsigned long int> degree_list = test_degree_list(i, N[j], gamma_values[i], lambda_values[1], kmin);
            std::cout <<"test_degree_list done!"<< std::endl;
            test_topology_conf(degree_list, i, N[j], kmin);
            std::cout <<"test_topology_conf done!"<< std::endl;
            calc_config(num_rep[j], i, N[j], gamma_values[i], lambda_values[0], kmin, noc, 7);
            std::cout <<"calc_config done!"<< std::endl;
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Done! duration: (" << duration.count() << " milliseconds)"<< std::endl;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_ini);
    std::cout << "Done! Total duration: (" << duration.count() << " milliseconds)"<< std::endl;
}

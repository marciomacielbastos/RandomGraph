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


void calc (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin) {
//    std::regex e ("[.]");
    std::string folder = "/home/marcio/Random-graph/output";
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
//    out_string = std::regex_replace(ss.str(), e, "-");
    q_Exponential q_exp = q_Exponential(lambda, q, 1, N - 1);
    Percolation p(&q_exp, N);
    p.percolation_molloy_reed(num_rep);
    p.write_random_vector(folder + out_string + ".txt");

}

void calc_t (unsigned int num_rep, int i, unsigned long int f, double gamma, double lambda, int kmin, int thread_id) {
//    std::regex e ("[.]");
    std::string folder = "/home/marcio/Random-graph/output";
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
//    out_string = std::regex_replace(ss.str(), e, "-");
    q_Exponential q_exp = q_Exponential(lambda, q, 1, N - 1);
    Percolation p(&q_exp, N, thread_id);
    p.percolation_molloy_reed(num_rep);
    p.write_random_vector(folder + out_string + ".txt");

}


int main(int argc, char *argv[]){
//    auto start = std::chrono::high_resolution_clock::now();
//    std::regex e ("[.]");
    unsigned long int f[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    /**********************************/
    /*            Parameters          */
    /**********************************/

    unsigned int num_rep = 100;
    int kmin = 1;
    double gamma_values[5] = {2.5, 3.0, 3.5, 4.0 , 4.5};
    double lambda_values[5] = {1.751, 0.551, 0.334, 0.26, 0.223};



    /***********************************************************/
    /*                  Set Paremeters String                  */
    /***********************************************************/
    std::vector<std::thread> thrds;
    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 7; j++) {
            thrds.push_back(std::move(std::thread(calc, num_rep, i, f[j], gamma_values[i], lambda_values[i], kmin)));
        }
    }

    for (auto& t : thrds) {
        t.join();
    }

    for (int i = 0; i < 5; i++) {

        for (int j = 7; j < 11; j++) {
            thrds.push_back(std::move(std::thread(calc, num_rep, i, f[j], gamma_values[i], lambda_values[i], kmin)));
        }
    }

    for (auto& t : thrds) {
        t.join();
    }

}

#include <QCoreApplication>
#include <iostream>
#include <string>

#include <q_exp_graph_generator.h>

/* ------------------------------------------------------------------------------------------------*/
/* --------------------------------------[ GRAPH GENERATION ]--------------------------------------*/
/* ------------------------------------------------------------------------------------------------*/
void graph_generation(double lambda,
                      double gamma,
                      unsigned long int kmin,
                      unsigned long int N,
                      unsigned long int num_rep,
                      unsigned long int from,
                      unsigned long int n_threads,
                      std::string output) {
    std::cout <<"Building "<< num_rep << " graphs " <<
                "(gamma: "<< gamma << " lambda: " << lambda <<
                " kmin: "<< kmin << " N: " << N <<
                "), on " << n_threads << " threads" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    q_Exp_graph_generator q_gen = q_Exp_graph_generator(gamma,lambda,kmin,N);
    q_gen.set_graph_folder(output);
    q_gen.generate_multiple_graphs(num_rep, n_threads, from);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
}
/* ------------------------------------------------------------------------------------------------*/
/* --------------------------------------[ GRAPH GENERATION ]--------------------------------------*/
/* ------------------------------------------------------------------------------------------------*/

int main(int argc, char** argv) {
    std::string output = "/home/marcio/Projects/RandomGraph/output/";
    graph_generation(0.01, 2.1, 2, 16384, 100, 0, 5, output);
}


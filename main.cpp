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




int main(int argc, char *argv[]){
    std::vector<unsigned long int> test;
    unsigned long int  N = static_cast<unsigned long int>(100);
    double gamma = 2.5;
    double lambda = 1.7;
    double q = (gamma + 1) / gamma;
    q_Exponential q_exp = q_Exponential(lambda, q, 1, N - 1);
    Percolation p(&q_exp, N);
    p.percolation_molloy_reed(100);
    p.write_random_vector("/home/marcio/Random-graph/output/teste.txt");
}

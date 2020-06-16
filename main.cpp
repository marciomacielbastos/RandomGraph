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

void write_random_vector(const std::string& filename, std::vector<unsigned long int> random_vector){
    std::ofstream myfile;
    myfile.open (filename);
    for (unsigned long int i=0; i < random_vector.size(); i++){
        if(i < random_vector.size() - 1) myfile << random_vector[i] << ",";
        else {
            myfile << random_vector[i];
        }
    }
    myfile.close();
}

void write_links(const std::string& filename, std::vector<std::pair<unsigned long int, unsigned long int>> list){
    std::ofstream myfile;
    myfile.open (filename);
    std::cout <<"[Writing graph links...]"<< std::endl;
//    myfile << "Source" << "," << "Target" << std::endl;
    while (!list.empty()) {
        std::pair<unsigned long int, unsigned long int> pair = list.back();
        if(list.size() > 1){
            myfile << pair.first << "," << pair.second << std::endl;
        } else{
            myfile << pair.first << "," << pair.second;
        }
        // Remove the link from the list of links
        list.pop_back();
    }
    myfile.close();
}


int main(int argc, char *argv[]){
//    std::bitset<1024> foo;
    std::vector<unsigned long int> test = {1, 1, 1, 1, 1, 3, 3, 5, 6, 8};
    Topology_builder tb = Topology_builder(test);
//    for(const int &i : test){
//        std::cout<< test[1] << std::endl;
//    }
//    unsigned long int  N = static_cast<unsigned long int>(10240000);
//    double gamma = 2.5;
//    double lambda = 1.7;
//    double q = (gamma + 1) / gamma;
//    q_Exponential q_exp = q_Exponential(lambda, q, 1, N);
//    unsigned long int cnt = 0;
//    for(int j = 0; j < 10240000; j++){
//        unsigned long int val = q_exp.randint();
//        if(val > 1){
//            cnt++;
//        }
//    std::cout << cnt << '\n';
//    }

//    std::vector<std::vector<bool>> bar;
//    for(unsigned long int j=0; j < 102400; j++){
//        std::vector<bool> foo;
//        for(int i =0 ; i < 102400; i++){
//            foo.push_back(true);
//        }
//        bar.push_back(foo);
//    }
//    Graph g(10240000);
//    char k =9;

//    unsigned long int  N = static_cast<unsigned long int>(1E1);
//    double gamma = 2.5;
//    double lambda = 1.7;
//    Percolation p = Percolation(N, lambda, gamma, 1);
//    std::vector<unsigned long int> random_vector = p.get_degree_list();
//    write_random_vector("/home/marcio/Random-graph/output/randon_vector.txt", random_vector);
//    write_links("/home/marcio/Random-graph/output/randon_vector.txt", p.get_link_list());
//    Graph g = Graph(degree_list);
//    std::deque<int> test;
//    test.push_back(1);
//    test.push_back(2);
//    test.push_back(3);
//    int temp = test[2];
//    test[2] = test[0];
//    test[0] = temp;
//    std::cout << (v) << std::endl;
//    for(int i=0; i < 3; i++){
//        std::cout << test[i] << std::endl;
//    }

//    std::vector<unsigned long int> random_vector;
//    for(int i=0; i < 100; i++){
//        int sum = 0;
//        for(int j = 0; j < 10000; j++){
//           sum += q.randint();
//        }
//        random_vector.push_back(sum);
//    }
//    write_random_vector("/home/marcio/Random-graph/output/randon_vector.txt", random_vector);
}

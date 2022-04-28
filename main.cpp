#include <QCoreApplication>
#include <iostream>
#include <map>
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
enum Options {
    Option1,
    Option2,
    Option3,
    Option4,
    Option5,
    Option6,
    Option7,
    Option8,
    Option_Invalid,
};


Options resolveOption(std::string input) {
   static const std::map<std::string, Options> optionStrings {
       { "-l", Option1 },
       { "-g", Option2 },
       { "-k", Option3 },
       { "-N", Option4 },
       { "-r", Option5 },
       { "-f", Option6 },
       { "-t", Option7 },
       { "-o", Option8 }};
   auto itr = optionStrings.find(input);
   if( itr != optionStrings.end() ) {
       return itr->second;
   }
   return Option_Invalid;
}

int main(int argc, char** argv) {
    /**********************************/
    /*            Parameters          */
    /**********************************/
    double lambda = 100;
    double gamma = 2.5;
    unsigned long int  kmin = 1;
    unsigned long int N = 512;
    unsigned long int num_rep = 1;
    unsigned long int from = 0;
    std::string output = "/home/marcio/Projects/RandomGraph/output/";
    unsigned long int n_threads = 1;
    /**********************************/
    /*            Parameters          */
    /**********************************/

    std::string arg;
    for (int i=1; i< argc; i++){
        arg = argv[i];
        if (arg == "-") {
            arg += argv[++i];
        }
        switch(resolveOption(arg)) {
            case Option1: lambda = std::stod(argv[++i]);
                          break;

            case Option2: gamma = std::stod(argv[++i]);
                          break;

            case Option3: kmin = std::stoul(argv[++i]);
                          break;

            case Option4: N = std::stoul(argv[++i]);
                          break;

            case Option5: num_rep = std::stoul(argv[++i]);
                          break;

            case Option6: from = std::stoul(argv[++i]);
                          break;

            case Option7: n_threads = std::stoul(argv[++i]);
                          break;

            case Option8: output = argv[++i];
                       break;

            case Option_Invalid: std::cout << "argv[0]: invalid option -- '" <<argv[i] << "'" << std::endl;
                                 return 0;
        }
    }
    std::cout << "lambda: (" << lambda << ")"<< std::endl;
    std::cout << "gamma: (" << gamma << ")"<< std::endl;
    std::cout << "kmin: (" << kmin << ")"<< std::endl;
    std::cout << "N: (" << N << ")"<< std::endl;
    std::cout << "num_rep: (" << num_rep << ")"<< std::endl;
    std::cout << "from: (" << from << ")"<< std::endl;
    std::cout << "subfolder: (" << output << ")"<< std::endl;
    graph_generation(lambda, gamma, kmin, N, num_rep, from, n_threads, output);
}


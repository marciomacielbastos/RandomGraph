#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <q_exp_graph_generator.h>
#include <percolation_random.h>


void progress_bar(unsigned long int i, double increment, int count) {
    double progress = increment * static_cast<double>(i);
    unsigned int bar_width = 60;
    std::cout << "[" << count << "]"<< "[";
    unsigned int pos = static_cast<unsigned int>(double(bar_width) * progress);
    for (unsigned int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<unsigned int>(progress * 100.0) << " %  \r";
    std::cout.flush();
    if(progress >= 1){
        std::cout << "\n";
    }
}

std::string get_filename(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned int i) {
    unsigned long int n = std::log2(N);
    std::string filename;
    std::stringstream ss;
    ss.str("");
    ss << gamma;
    ss << "_" << lambda;
    ss << "_" << kmin;
    ss << "_" << n;
    if (i < 10)  ss << "_00";
    else if (i < 100) ss << "_0";
    else ss << "_";
    ss << i;
    filename = ss.str() + ".txt";
    return filename;
}

/* -----------------------------------------------------------------------------------------------*/
/* --------------------------------------[ DEGREES WRITING ]--------------------------------------*/
/* -----------------------------------------------------------------------------------------------*/
void degrees_writing(double lambda,
                     double gamma,
                     unsigned long int kmin,
                     unsigned long int N,
                     unsigned long int num_rep,
                     unsigned long int from,
                     std::string source,
                     std::string output) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "DEGREES WRITING" << std::endl;
    std::string fname;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i + from);
        progress_bar(i, increment, 1);
        Graph G (N);
        G.read_file(source + fname, ' ');
        G.save_degrees(output + fname);
    }
    progress_bar(num_rep, increment, 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
}
/* -----------------------------------------------------------------------------------------------*/
/* --------------------------------------[ DEGREES WRITING ]--------------------------------------*/
/* -----------------------------------------------------------------------------------------------*/

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

/* ------------------------------------------------------------------------------------------------*/
/* ----------------------------------------[ RANDOM ATTACK ]---------------------------------------*/
/* ------------------------------------------------------------------------------------------------*/
void random_attack(double lambda,
                   double gamma,
                   unsigned long int kmin,
                   unsigned long int N,
                   unsigned long int num_rep,
                   unsigned long int from,
                   double begin,
                   double end,
                   std::string source,
                   std::string output) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Random Attack " << std::endl;
    std::string fname;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i + from);
        progress_bar(i, increment, 1);
        Graph G (N);
        G.read_file(source + fname, ' ');
        Percolation_random p = Percolation_random();
        p.percolate_on_the_interval(G, begin, end, 200);
        p.save(output + fname);
    }
    progress_bar(num_rep, increment, 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
}
/* ------------------------------------------------------------------------------------------------*/
/* ----------------------------------------[ RANDOM ATTACK ]---------------------------------------*/
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
    Option9,
    Option10,
    Option11,
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
       { "-s", Option8 },
       { "-o", Option9 },
       { "-b", Option10 },
       { "-e", Option11 }};
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
    double lambda = 1;
    double gamma = 3.7;
    double begin = 0.6;
    double end = 1;
    unsigned long int  kmin = 2;
    unsigned long int N = 100;
    unsigned long int num_rep = 1;
    unsigned long int from = 0;
    std::string source = "/home/marcio/Projects/RandomGraph/output/Teste/";
    std::string output = "./";
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

            case Option8: source = argv[++i];
                          break;

            case Option9: output = argv[++i];
                          break;

            case Option10: begin = std::stod(argv[++i]);
                      break;

            case Option11: end = std::stod(argv[++i]);
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
    std::cout << "source: (" << source << ")"<< std::endl;
    std::cout << "output: (" << output << ")"<< std::endl;
//    degrees_writing(lambda, gamma, kmin, N, num_rep, from, source, output);
//    graph_generation(lambda, gamma, kmin, N, num_rep, from, n_threads, output);
    random_attack(lambda, gamma, kmin, N, num_rep, from, begin, end, source, output);
}


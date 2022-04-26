#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>

/* Show duration time of computation */
#include <chrono>
#include <time.h>

#include <betweenness.h>
#include <clustering_coefficient.h>
#include <integrity_analyser.h>
#include <percolation_vertex.h>
#include <percolation_degree.h>
#include <poisson.h>
#include <q_exp_graph_generator.h>
#include <sis_model.h>

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
        Percolation_vertex p = Percolation_vertex();
        p.percolate_on_the_interval(G, begin, end, 200);
        p.percolate_molloy_reed(G);
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

/* ------------------------------------------------------------------------------------------------*/
/* --------------------------------------[ MALICIOUS ATTACK ]--------------------------------------*/
/* ------------------------------------------------------------------------------------------------*/
void malicious_attack(double lambda,
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
    std::cout << "Malicious Attack " << std::endl;
    std::string fname;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i + from);
        progress_bar(i, increment, 1);
        Graph G (N);
        G.read_file(source + fname, ' ');
        Percolation_degree p = Percolation_degree();
        p.percolate_on_the_interval(G, begin, end, 200);
        p.percolate_molloy_reed(G);
        p.save(output + fname);
    }
    progress_bar(num_rep, increment, 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
}
/* ------------------------------------------------------------------------------------------------*/
/* --------------------------------------[ MALICIOUS ATTACK ]--------------------------------------*/
/* ------------------------------------------------------------------------------------------------*/


int main(int argc, char** argv){
    /**********************************/
    /*            Parameters          */
    /**********************************/
    double lambda = std::stod(argv[1]);
    double gamma = std::stod(argv[2]);
    unsigned long int kmin = std::stoul(argv[3]);
    unsigned long int N = std::stoul(argv[4]);
    unsigned long int num_rep = std::stoul(argv[5]);
    unsigned long int from = std::stoul(argv[6]);
    std::string subfolder = argv[7];
/*******************************************************/
//    unsigned long int n_threads = std::stoul(argv[8]);
/*******************************************************/
//    double begin = std::stod(argv[8]);
//    double end = std::stod(argv[9]);
/*******************************************************/
//    double lambda = 100;
//    double gamma = 2.125;
//    unsigned long int  kmin = 1;
//    unsigned long int N = 16384;
//    unsigned long int num_rep = 1;
//    unsigned long int from = 0;
//    std::string subfolder = "16k";
//    double begin = 0;
//    double end = 0.25;
//    unsigned long int n_threads = 1;
    /**********************************/
    /*            Parameters          */
    /**********************************/

    std::cout << "lambda: (" << lambda << ")"<< std::endl;
    std::cout << "gamma: (" << gamma << ")"<< std::endl;
    std::cout << "kmin: (" << kmin << ")"<< std::endl;
    std::cout << "N: (" << N << ")"<< std::endl;
    std::cout << "num_rep: (" << num_rep << ")"<< std::endl;
    std::cout << "from: (" << from << ")"<< std::endl;
    std::cout << "subfolder: (" << subfolder << ")"<< std::endl;
//    std::cout << "begin_interval: (" << begin << ")"<< std::endl;
//    std::cout << "end_interval: (" << end << ")"<< std::endl;
//    std::cout << "n_threads: (" << n_threads << ")"<< std::endl;

    /**********************************/
    /*             Folders            */
    /**********************************/

    std::string folder_simulated_graphs = "/home/marcio/RandonGraph/Random-graph/output/simulated_graphs/q_exponential/" + subfolder + "/";
    std::string folder_simulated_degrees = "/home/marcio/RandonGraph/Random-graph/output/degrees/" + subfolder + "/";
    std::string folder_random_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/vertices/" + subfolder + "/";
    std::string folder_mal_attack = "/home/marcio/RandonGraph/Random-graph/output/percolation/degree/" + subfolder + "/";
    std::string folder_clustering_coefficient = "/home/marcio/RandonGraph/Random-graph/output/clustering_coefficient/"+ subfolder + "/";
    std::string folder_betweenness = "/home/marcio/RandonGraph/Random-graph/output/betweenness/"+ subfolder + "/";

//    graph_generation(lambda, gamma, kmin, N, num_rep, from, n_threads, folder_simulated_graphs);
    degrees_writing(lambda, gamma, kmin, N, num_rep, from, folder_simulated_graphs, folder_simulated_degrees);
//    random_attack(lambda, gamma, kmin, N, num_rep, from, begin, end, folder_simulated_graphs, folder_random_attack);
//    malicious_attack(lambda, gamma, kmin, N, num_rep, from, begin, end, folder_simulated_graphs, folder_mal_attack);

    /* ------------------------------------------------------------------------------------------------*/
    /* -----------------------------------[ BETWEENNESS CENTRALITY ]-----------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/
    /*
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Betweenness Centrality " << std::endl;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i + from);
        progress_bar(i, increment, 1);
        Graph G (N);
        G.read_file(folder_simulated_graphs + fname, ',');
        Betweenness BC = Betweenness(G);
        BC.set_num_of_threads(n_threads);
        BC.betweenness_centrality();
        BC.save(folder_betweenness + fname);
    }
    progress_bar(num_rep, increment, 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
    */
    /* ------------------------------------------------------------------------------------------------*/
    /* -----------------------------------[ BETWEENNESS CENTRALITY ]-----------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/

    /* ------------------------------------------------------------------------------------------------*/
    /* -----------------------------------[ CLUSTERING COEFFICIENT ]-----------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/
    /*
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Clustering coefficient " << std::endl;
    double increment = 1 / static_cast<double>(num_rep);
    for (unsigned long int i = 0; i < num_rep; i++) {
        fname = get_filename(gamma, lambda, kmin, N, i + from);
        progress_bar(i, increment, 1);
        Graph G (N);
        G.read_file(folder_simulated_graphs + fname, ',');
        Clustering_coefficient C = Clustering_coefficient(G);
        C.clustering_multithread(n_threads);
        C.save(folder_clustering_coefficient + fname);
    }
    progress_bar(num_rep, increment, 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
    */
    /* ------------------------------------------------------------------------------------------------*/
    /* -----------------------------------[ CLUSTERING COEFFICIENT ]-----------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/

    /* ------------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ INTEGRITY ANALYSIS ]------------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/
    /*
    double gamma;
    double xi, Mi, Mi_1, Si = 0;
    std::ofstream myfile ("integrity.txt");
    std::cout << "Integrity Analysis " << std::endl;
    for (int i=0; i < 27; i++) {
        gamma = 2.5 + (0.25 * static_cast<double>(i));
        Integrity_analyser ia(N);
        for (unsigned long int j = 0; j < num_rep; j++) {
             fname = get_filename(gamma, lambda, kmin, N, j + from);
             ia.read_graph(folder_simulated_degrees + fname, ',');
             xi = (static_cast<double>(ia.get_max()) / static_cast<double>(N)) * 100.0;
             Mi_1 = Mi;
             Mi = Mi + (xi - Mi) / (j + 1);
             Si = Si + (xi - Mi_1) * (xi - Mi);
             ia.flush();
        }
        if (myfile.is_open()) {
            myfile << gamma <<" "<< Mi << " " << Si / static_cast<double>(num_rep - 1) << std::endl;
            std::cout << gamma <<" "<< Mi << " " << Si / (static_cast<double>(num_rep - 1) * 10) << std::endl;
        }
        else std::cout << "Unable to open file";
    }
    myfile.close();
    /*
    /* ------------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ INTEGRITY ANALYSIS ]------------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/

    /* ------------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ DEGREE ANALYSIS ]------------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/
    /*
    double gamma;
    double xi, Mi, Mi_1, Si = 0;
    std::ofstream myfile ("mean_degree.txt");
    std::cout << "Degree Analysis " << std::endl;
    std::vector<unsigned long int> degrees;
    for (int i=0; i < 27; i++) {
        gamma = 2.5 + (0.25 * static_cast<double>(i));
        for (unsigned long int j = 0; j < num_rep; j++) {
             fname = get_filename(gamma, lambda, kmin, N, j + from);
             Graph g(N);
             g.read_file(folder_simulated_graphs + fname, ',');
             degrees = g.get_degrees();
             xi = 0;
             while (!degrees.empty()) {
                 xi += degrees.back();
                 degrees.pop_back();
             }
             xi = xi / static_cast<double>(N);
             Mi_1 = Mi;
             Mi = Mi + (xi - Mi) / (j + 1);
             Si = Si + (xi - Mi_1) * (xi - Mi);
        }
        if (myfile.is_open()) {
            myfile << gamma <<" "<< Mi << Si / (static_cast<double>(num_rep - 1) * 10)  << std::endl;
            std::cout << gamma <<" "<< Mi << " " << Si / (static_cast<double>(num_rep - 1) * 10) << std::endl;
        }
        else std::cout << "Unable to open file";
    }
    myfile.close();
    */
    /* ------------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ INTEGRITY ANALYSIS ]------------------------------------*/
    /* ------------------------------------------------------------------------------------------------*/

    /* --------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ SIS ANALYSIS ]--------------------------------------*/
    /* --------------------------------------------------------------------------------------------*/
    /*
    unsigned long int number_of_years = 1;
    unsigned long int iterations = (365 * number_of_years) + (number_of_years / 4);
    double increment = 1 / static_cast<double>(iterations);
    double probability_to_infect;
    double mean_infectious_interval = 5;
    int count = 100;
    Graph g = q_gen.generate_graph();
    Sis_model sis;
    sis.set_graph(g);
    sis.set_mean_infectious_interval(mean_infectious_interval);
    while (count > 0) {
        probability_to_infect = 0.125;
        sis.restart();
        for (int i = 0; i < 11; i++) {
            sis.set_probability_to_infect(probability_to_infect);
            for (unsigned long int j = 0; j < iterations; j++) {
                progress_bar(j, increment, 100 - count);
                if (sis.infectious_is_empty()) {
                    sis.randomly_infect();
                }
                sis.record_number_of_infecteds();
                sis.run_dynamics();
            }
            probability_to_infect /= 2;
        }
        sis.save("infeccao_N"+std::to_string(N)+"_g2.5_l0.1_descendo_" + std::to_string(100 - count) + ".txt", " ");
        count--;
    }
    count = 100;
    double PROBABILITY = std::pow(2.0, -12);
    while (count > 0) {
        probability_to_infect = PROBABILITY;
        sis.restart();
        for (int i = 0; i < 11; i++) {
            sis.set_probability_to_infect(probability_to_infect);
            for (unsigned long int j = 0; j < iterations; j++) {
                progress_bar(j, increment, count);
                if (sis.infectious_is_empty()) {
                    sis.randomly_infect();
                }
                sis.record_number_of_infecteds();
                sis.run_dynamics();//    auto start = std::chrono::high_resolution_clock::now();
    //    std::cout <<"Malicious Attack "<< "gamma: "<< gamma << " lambda: " << lambda << " kmin: "<< kmin << " N: " << N << ", " << num_rep << " times, on " << n_threads << " cores" << std::endl;
        std::cout << "Malicious Attack " << std::endl;
        double increment = 1 / static_cast<double>(num_rep);
        for (unsigned long int i = 0; i < num_rep; i++) {
            fname = get_filename(gamma, lambda, kmin, N, i + from);
            progress_bar(i, increment, 1);
            Graph G (N);
            G.read_file(folder_simulated_graphs + fname, ',');
            Percolation_degree p = Percolation_degree();
            p.percolate_on_the_interval(G, begin_interval, end_interval, 200);
            p.percolate_molloy_reed(G);
            p.save(folder_mal_attack + fname);
    //        Betweenness BC = Betweenness(G);
    //        BC.set_num_of_threads(n_threads);
    //        BC.betweenness_centrality();
    //        BC.save(folder_betweenness + fname);
    //        Clustering_coefficient C = Clustering_coefficient(G);
    //        C.clustering_multithread(n_threads);
    //        C.save(folder_clustering_coefficient + fname);
        }
        progress_bar(num_rep, increment, 1);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Done! duration: (" << duration.count() << " seconds)"<< std::endl;
            }
            probability_to_infect *= 2;
        }
        sis.save("infeccao_N"+std::to_string(N)+"_g2.5_l0.1_subindo_" + std::to_string(100 - count) + ".txt", " ");
        count--;
    }
    */
    /* --------------------------------------------------------------------------------------------*/
    /* --------------------------------------[ SIS ANALYSIS ]--------------------------------------*/
    /* --------------------------------------------------------------------------------------------*/


}

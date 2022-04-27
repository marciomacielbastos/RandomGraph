#include "q_exp_graph_generator.h"

q_Exp_graph_generator::q_Exp_graph_generator(double gamma, double lambda, unsigned long kmin, unsigned long N) {
    this->gamma = gamma;
    this->lambda = lambda;
    this->kmin = kmin;
    this->N = N;
    this->filename_prefix = make_filename_prefix();
    this->degrees_folder = "";
    this->graph_folder = "./";
}

void q_Exp_graph_generator::set_degrees_folder(std::string folder) {
    this->degrees_folder = folder;
}

void q_Exp_graph_generator::set_graph_folder(std::string folder) {
    this->graph_folder = folder;
}


std::string q_Exp_graph_generator::make_filename_prefix() {
    unsigned long int n = std::log2(this->N);
    std::stringstream ss;
    ss.str("");
    ss << gamma;
    ss << "_" << lambda;
    ss << "_" << kmin;
    ss << "_" << n;
    this->filename_prefix = ss.str();
    return this->filename_prefix;
}

std::string q_Exp_graph_generator::make_filename_suffix(unsigned int id) {
    std::stringstream ss;
    int str_size;
    ss.str("");
    ss << id;
    str_size = ss.str().length();
    int number_of_zeros = std::max(0, 3 - str_size);
    std::string i_with_leading_zeros = "_" + std::string(number_of_zeros, '0') + ss.str();
    return i_with_leading_zeros;
}

void q_Exp_graph_generator::generate_multiple_graphs(unsigned long int repetitions, unsigned long int n_threads) {
    generate_multiple_graphs(repetitions, n_threads, 0);
}

double q_Exp_graph_generator::calculate_tsallis_q() {
    double q = (this->gamma + 1) / this->gamma;
    return q;
}

std::vector<unsigned long int> q_Exp_graph_generator::sample_from_dist(Probability_distribution *dist) {
    std::vector<unsigned long int> degree_list;
    unsigned long int N = this->N;
    while (N > 0){
        unsigned long int val = dist->randint();
        degree_list.push_back(val);
        N--;
    }
    return degree_list;
}

std::vector<unsigned long int> q_Exp_graph_generator::sample_degrees(){
    std::vector<unsigned long int> degrees_vector;
    Probability_distribution *dist;
    if (this->lambda > 0) {
        double q = calculate_tsallis_q();
        dist = new qExponential(this->lambda, q, this->kmin, this->N);
    }
    else {
        dist = new PowerLaw(this->gamma, this->kmin, this->N);
    }

    degrees_vector = sample_from_dist(dist);
    return degrees_vector;
}

void q_Exp_graph_generator::write_degrees(Graph G, std::string filepath) {
    std::vector<unsigned long int> degrees_vector = G.get_degrees();
    std::ofstream myfile;
    myfile.open (filepath);
    for(unsigned long int i = 0;  i < degrees_vector.size(); i++ ){
        if(i < degrees_vector.size() - 1) myfile << degrees_vector[i] << ",";
        else {
            myfile << degrees_vector[i] << std::endl;
        }
    }
    myfile.close();
}


std::string q_Exp_graph_generator::make_degree_filepath(unsigned int id) {
    std::string fname = this->filename_prefix + make_filename_suffix(id);
    return this->degrees_folder + fname + ".txt";
}

std::string q_Exp_graph_generator::make_graph_filepath(unsigned int id) {
    std::string fname = this->filename_prefix + make_filename_suffix(id);
    return this->graph_folder + fname + ".txt";
}

Graph q_Exp_graph_generator::calc_topology_with_configurational_method(std::vector<unsigned long int> degree_list) {
    Configurational_model tb = Configurational_model(degree_list);
    tb.random_link();
    Graph G = tb.get_g();
    return G;
}

Graph q_Exp_graph_generator::generate_graph(std::vector<unsigned long int> degrees_vector) {
    Graph G = calc_topology_with_configurational_method(degrees_vector);
    return G;
}

Graph q_Exp_graph_generator::generate_graph() {
    std::vector<unsigned long int> degrees_vector = sample_degrees();
    Graph G = generate_graph(degrees_vector);
    return G;
}

void q_Exp_graph_generator::graph_build_and_save(unsigned int id) {
    std::vector<unsigned long int> degrees_vector = sample_degrees();
    Graph G = generate_graph(degrees_vector);
    std::string filepath = make_graph_filepath(id);
    G.save_graph(filepath);
    if (this->degrees_folder.length() > 0) {
        filepath = make_degree_filepath(id);
        write_degrees(G, filepath);
    }
}


void q_Exp_graph_generator::graph_build_and_save_on_thread(std::vector<std::thread> &threads, unsigned int id) {
    std::thread t (&q_Exp_graph_generator::graph_build_and_save, this, id);
    threads.push_back(std::move(t));
}

void q_Exp_graph_generator::flush_threads(std::vector<std::thread> &threads) {
    if (threads.size() > 0) {
        for (unsigned long int j = 0; j < threads.size(); j++) {
            threads[j].join();
        }
    }
    while (!threads.empty()) {
        threads.pop_back();
    }
}

void q_Exp_graph_generator::generate_multiple_graphs(unsigned long int repetitions, unsigned long int n_threads, unsigned long int from) {
    std::vector<std::thread> threads;
    double increment = 1 / static_cast<double>(repetitions);
    unsigned long int num_graphs_built = 0;
//    std::cout <<"Building "<< repetitions << " graphs " <<
//                "(gamma: "<< this->gamma << " lambda: " << this->lambda <<
//                " kmin: "<< this->kmin << " N: " << this->N <<
//                "), on " << n_threads << " threads" << std::endl;
    for (unsigned long int i = 0; i < repetitions; i++) {
        progress_bar(num_graphs_built, increment);
        if ((i + 1) % n_threads) {
            graph_build_and_save_on_thread(threads, i + from);
        }
        else {
            graph_build_and_save(i + from);
            flush_threads(threads);
            num_graphs_built += n_threads;
        }
    }
    progress_bar(repetitions, increment);
    flush_threads(threads);
}

void q_Exp_graph_generator::progress_bar(unsigned long int i, double increment) {
    double progress = increment * static_cast<double>(i);
    unsigned int bar_width = 70;
    std::cout << "[";
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

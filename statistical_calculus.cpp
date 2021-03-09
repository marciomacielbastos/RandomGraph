#include "statistical_calculus.h"

Statistical_calculus::Statistical_calculus() {

}

void Statistical_calculus::mean_std_result(unsigned long int i,
                           std::vector<std::vector<double>>& mean_results,
                           std::vector<double> observed_values) {
    double pc_mu;
    if (mean_results.empty()) {
        std::vector<std::vector<double>> result(observed_values.size(), {0,0});
        mean_results = result;
    }

    while (mean_results.size() < observed_values.size()) {
        mean_results.push_back({0, 0});
    }

    while (mean_results.size() > observed_values.size()) {
        observed_values.push_back(0);
    }
    for (unsigned int j = 0; j < observed_values.size(); ++j) {
        pc_mu = mean_results[j][0];
        //Mean
        mean_results[j][0] = mean_results[j][0] + ((observed_values[j] - mean_results[j][0]) / (i + 1));
        //Var
        mean_results[j][1] = mean_results[j][1] + (observed_values[j] - pc_mu) * (observed_values[j] - mean_results[j][0]);
    }
}

void Statistical_calculus::mean_std_other_results(unsigned long int i,
                           std::vector<double>& mean_results,
                           std::vector<double> observed_values) {
    double pc_mu;
    if (mean_results.empty()) {
        for (unsigned int j = 0; j < observed_values.size(); ++j) {
            mean_results.push_back(0);
            mean_results.push_back(0);
        }
    }

    for (unsigned int j = 0; j < observed_values.size(); ++j) {
        pc_mu = mean_results[j];
        //Mean
        mean_results[j] = mean_results[j] + ((observed_values[j] - mean_results[j]) / (i + 1));
        //Var
        mean_results[j + 2] = mean_results[j + 2] + (observed_values[j] - pc_mu) * (observed_values[j] - mean_results[j]);
    }
}

void Statistical_calculus::progress_bar(unsigned long int i) {
    double progress = this->increment * static_cast<double>(i);
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

void Statistical_calculus::progress_bar(unsigned long int i, double increment) {
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

void Statistical_calculus::add_percolation(Percolation *p) {
    this->percolations.push_back(p);
}


std::string Statistical_calculus::get_filename(std::string folder, double gamma, double lambda, unsigned long kmin, unsigned long n, unsigned long int i) {
    std::stringstream filename;
    filename.str("");
    filename << folder;
    filename << gamma;
    filename << "_" << lambda;
    filename << "_" << kmin;
    filename << "_" << n;
    filename << "_" << i;
    filename << ".txt";
    return filename.str();
}

void Statistical_calculus::percolate(Percolation *p, Graph &G) {
    p->percolate(G);
}

void Statistical_calculus::calc_statistics(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned long int number_of_samples, std::string folder) {
    if (this->percolations.size() < 1) {
        std::cout<< "No percolation added" << std::endl;
        return;
    }
    double n = std::log2(N);
    this->increment = 1 / static_cast<double>(number_of_samples);
    unsigned int n_threads = this->percolations.size() - 1;
    std::string filename;
    std::vector<double> observation;
    std::vector<double> other_realizations;
    if (this->results.empty()) {
        for (unsigned int i =0; i <= n_threads; i++) {
            std::vector<std::vector<double>> r;
            std::vector<std::vector<double>> _or;
            this->results.push_back(r);
            this->other_results.push_back(_or);
        }
    }

    for (unsigned long int i = 0; i < number_of_samples; i++) {
        progress_bar(i);
        // Setting up
        Graph G (N);
        filename = get_filename(folder, gamma, lambda, kmin, n, i);
        G.read_file(filename, ',');

        // Threading
        std::vector<std::thread> threads;
        Percolation **p = this->percolations.data();
        for (unsigned int j=0; j < n_threads; j++) {
            std::thread t (&Statistical_calculus::percolate, this,*p, std::ref(G));
            threads.push_back(std::move(t));
            ++p;
        }
        (*p)->percolate(G);

        for (unsigned int j=0; j < n_threads; j++) {
            threads[j].join();
        }

        while (!threads.empty()) {
            threads.pop_back();
        }
        // Results
        p = this->percolations.data();
        for (unsigned int j=0; j <= n_threads; j++) {
            observation = (*p)->get_result();
            other_realizations = (*p)->get_other_result();
            mean_std_result(i, this->results[j], observation);
            mean_std_result(i, this->other_results[j], other_realizations);
            (*p)->flush();
            ++p;
        }
    }
    progress_bar(number_of_samples);
}

void Statistical_calculus::calc(std::string filepath, unsigned long int N) {
    std::vector<std::vector<double>> realization;
    std::vector<std::vector<double>> other_realization;
    if (this->percolations.size() < 1) {
        std::cout<< "No percolation added" << std::endl;
        return;
    }
    unsigned int n_threads = this->percolations.size() - 1;
    if (this->results.empty()) {
        for (unsigned int i =0; i <= n_threads; i++) {
            std::vector<std::vector<double>> r;
            std::vector<std::vector<double>> _or;
            this->results.push_back(r);
            this->other_results.push_back(_or);
        }
    }
    // Setting up
    Graph G (N);
    G.read_file(filepath, ',');

    // Threading
    std::vector<std::thread> threads;
    Percolation **p = this->percolations.data();
    for (unsigned int j=0; j < n_threads; j++) {
        std::thread t (&Statistical_calculus::percolate, this,*p, std::ref(G));
        threads.push_back(std::move(t));
        ++p;
    }
    (*p)->percolate(G);

    for (unsigned int j=0; j < n_threads; j++) {
        threads[j].join();
    }

    while (!threads.empty()) {
        threads.pop_back();
    }
    // Results
    p = this->percolations.data();
    for (unsigned int j=0; j <= n_threads; j++) {
       realization.push_back((*p)->get_result());
       other_realization.push_back((*p)->get_other_result());
        (*p)->flush();
        ++p;
    }
    this->realization = realization;
    this->other_realization = other_realization;
}

std::vector<std::vector<std::vector<double>>> Statistical_calculus::get_results() {
    return this->results;
}

std::vector<std::vector<std::vector<double>>> Statistical_calculus::get_other_results() {
    return this->other_results;
}

std::vector<std::vector<double>> Statistical_calculus::get_realization() {
    return this->realization;
}

std::vector<std::vector<double>> Statistical_calculus::get_other_realization() {
    return this->other_realization;
}

std::string Statistical_calculus::get_percolation_name(unsigned int i) {
    return this->percolations[i]->get_name();
}

std::string Statistical_calculus::get_percolation_other(unsigned int i) {
    return this->percolations[i]->get_other();
}

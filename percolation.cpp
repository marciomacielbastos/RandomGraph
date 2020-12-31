#include "percolation.h"
Percolation::Percolation(){}

Percolation::Percolation(Distribution * probability_distribution, unsigned long int  N, unsigned long int  noc) {
    this->probability_distribution = probability_distribution;
    this->N = N;
    this->noc = noc;
}

void Percolation::progress_bar(double increment, unsigned long int i, unsigned long int n) {
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
    if(i == n){
        std::cout << "\n";
    }
}

double Percolation::get_q(double gamma) {
    return (gamma + 1) / gamma;
}

std::vector<unsigned long int> Percolation::get_degree_list() {
    std::vector<unsigned long int> degree_list;
    for (unsigned long int j = 0; j < this->N; j++) {
        unsigned long int val = this->probability_distribution->randint();
        degree_list.push_back(val);
    }
    return degree_list;
}

std::vector<unsigned long int> Percolation::get_modified_degree_list(){
    std::vector<unsigned long int> degree_list;
    unsigned long int j = 0;
    while (j < this->N) {
        unsigned long int val = this->probability_distribution->randint();
        if (val > 1) {
            degree_list.push_back(val);
            j++;
        }
    }
    return degree_list;
}

void Percolation::t_geodesical_distance(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix) {
    Bfs bfs = Bfs();
    unsigned long int number_of_samples = 1000;
    number_of_samples = std::min(number_of_samples, this->N);
    mean_l += bfs.avg_geo_dist(number_of_samples, adj_matrix);
}

void Percolation::update_percolation_list(unsigned long n, unsigned long j,
                                          std::vector<std::vector<double>>& results,
                                          std::vector<double> input) {

    if (results.size() < j + 1) {
        results.push_back({0, 0, 0, 0});
    }
    double pc_mu = results[j][0];
    double bc_mu = results[j][1];
    //Mean
    results[j][0] = results[j][0] + ((input[0] - results[j][0]) / (n + 1));
    results[j][1] = results[j][1] + ((input[1] - results[j][1]) / (n + 1));
    //Var
    results[j][2] = results[j][2] + (input[0] - pc_mu) * (input[0] - results[j][0]);
    results[j][3] = results[j][3] + (input[1] - bc_mu) * (input[1] - results[j][1]);
}

void Percolation::update_percolation_list(unsigned long n, unsigned long j,
                                          std::vector<std::vector<double>>& results,
                                          double input) {
    if (results.size() < j + 1) {
        results.push_back({0, 0});
    }

    double pc_mu = results[j][0];
    //Mean
    results[j][0] = results[j][0] + ((input - results[j][0]) / (n + 1));
    //Var
    results[j][1] = results[j][1] + (input - pc_mu) * (input - results[j][0]);
}

void Percolation::mean_std_percolation(unsigned long int n,
                                       std::vector<std::vector<double>>& results,
                                       std::vector<std::vector<double>> input) {

    for (unsigned int j = 0; j < input.size() ; ++j) {
        update_percolation_list(n, j, results, input[j]);
    }
}

void Percolation::mean_std_percolation(unsigned long int n,
                                       std::vector<std::vector<double>>& results,
                                       std::vector<double> input) {
    for (unsigned int j = 0; j < input.size() ; ++j) {
        update_percolation_list(n, j, results, input[j]);
    }

}

void Percolation::percolation_configurational(unsigned int number_of_samples, unsigned long int n_threads) {
    std::vector<double> partial_result;
    std::vector<std::vector<double>> partial_result_pe;
//    std::vector<std::vector<double>> pb_result(this->noc, {0,0});
    double mean_l;
    std::vector<std::vector<double>> pd_result(this->N, {0,0});
    std::vector<std::vector<double>> pk_result(this->N, {0,0});
    std::vector<std::vector<double>> pe_result(this->noc, {0,0,0,0});
    double increment = 1 / static_cast<double>(number_of_samples);
    for (unsigned long int n = 0; n < number_of_samples; n++) {
        progress_bar(increment, n,  number_of_samples);

        std::vector<unsigned long int> degree_list = get_modified_degree_list();

        TopologyBuilderConfigurational tb = TopologyBuilderConfigurational(degree_list);
        tb.random_link();

        while (!tb.is_built()) {
            degree_list = get_modified_degree_list();
            tb = TopologyBuilderConfigurational(degree_list);
            tb.random_link();
        }
        Graph G = tb.get_g();

//        Percolation_betweenness perc_betweenness;
//        Percolation_betweenness *pb;
//        pb = &perc_betweenness;
        Percolation_degree perc_degree;
        Percolation_degree *pd;
        pd = &perc_degree;
        Percolation_kcore perc_kcore;
        Percolation_kcore *pk;
        pk = &perc_kcore;
        Percolation_edge pe(this->noc);

//        std::thread t1 (&Percolation_betweenness::percolation_betweenness, pb, std::ref(G), n_threads);
        std::thread t1 (&Percolation::t_geodesical_distance, this, std::ref(mean_l), G.get_adj_matrix());
        std::thread t2 (&Percolation_degree::malicious_attack, pd, std::ref(G));
        std::thread t3 (&Percolation_kcore::kcore_decomposition, pk, std::ref(G));
        pe.percolation_molloy_reed_criterion(G);

        t1.join();
        t2.join();
        t3.join();

//        partial_result = pb->get_result();
//        this->mean_l += pb->get_mean_l();
//        mean_std_percolation(n, pb_result, partial_result);
        partial_result = pd->get_result();
        mean_std_percolation(n, pd_result, partial_result);
        partial_result = pk->get_result();
        mean_std_percolation(n, pk_result, partial_result);
        partial_result_pe = pe.get_result();
        mean_std_percolation(n, pe_result, partial_result_pe);
    }
    this->mean_l = mean_l / number_of_samples;
//    this->pb_result = pb_result;
    this->pk_result = pk_result;
    this->pd_result = pd_result;
    this->pe_result = pe_result;
    progress_bar(increment, number_of_samples,  number_of_samples);
}

std::vector<std::vector<double>> Percolation::get_betweenness_result() {
    return this->pb_result;
}

std::vector<std::vector<double>> Percolation::get_kcore_result() {
    return this->pk_result;
}

std::vector<std::vector<double>> Percolation::get_degree_result() {
    return this->pd_result;
}

std::vector<std::vector<double>> Percolation::get_edge_result() {
    return this->pe_result;
}

double Percolation::get_mean_l() {
    return this->mean_l;
}

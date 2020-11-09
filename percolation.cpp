#include "percolation.h"

Percolation::Percolation(Distribution * probability_distribution, unsigned long int  N, unsigned long int  noc) {
    this->probability_distribution = probability_distribution;
    this->N = N;
    this->noc = noc;
}

Percolation::Percolation(Distribution * probability_distribution, unsigned long int  N, unsigned long int  noc, int thread_id) {
    this->probability_distribution = probability_distribution;
    this->N = N;
    this->noc = noc;
    this->thread_id = thread_id;
}

void Percolation::progress_bar(double increment, unsigned long int i, unsigned long int n) {
//    for (int i=0; i<5; i++) std::cout << "\e[A";
//    for (int i=0; i<this->thread_id; i++) std::cout << "\e[B";
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
/*******************************************************************/
/*                         Calculus of <l>                         */
/*******************************************************************/

void Percolation::t_geodesical_distance(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix){
    Bfs bfs = Bfs();
    unsigned long int number_of_samples = 1000;
    number_of_samples = std::min(number_of_samples, this->N);
    mean_l += bfs.avg_geo_dist(number_of_samples, adj_matrix);
}

/*****************************************************************/
/*                       K-core decomposition                    */
/*****************************************************************/
void Percolation::get_k_core(unsigned long int k,
                             std::vector<std::pair<unsigned long, unsigned long>> &id_degree_vect,
                             std::vector<std::vector<unsigned long int>> adj_matrix) {

    for (std::vector<std::pair<unsigned long, unsigned long>>::reverse_iterator i = id_degree_vect.rbegin(); i != id_degree_vect.rend(); ++i ) {
        if(i->second <= k) {

        }
    }
}


std::vector<unsigned long int> Percolation::k_core_decomposition(Graph & g) {
    unsigned long int size = g.get_adj_matrix().size();
    std::vector<unsigned long int> degrees;
    std::vector<std::pair<unsigned long int, unsigned long int>> network_size;
    for (unsigned long int i = 0; i < size; ++i) {
        degrees.push_back(g.get_adj_matrix()[i].size());
    }
    Heap heap(degrees);
    unsigned long int k = 1;
    while (heap.size() > 0) {
        root_degree = heap.get_root_degree();
        while (root_degree <= k) {
            root = heap.pop();
            for (unsigned long int i = 0; i < g.get_adj_matrix()[root].size(); ++i) {
                heap.update(g.get_adj_matrix()[root][i]);
            }
        }
        std::pair<unsigned long int, unsigned long int> p(k, heap.size());
        network_size.push_back(p);
        k++;
    }
}


/******************************************************************/
/*                         Calculus of pc                         */
/******************************************************************/
std::vector<std::vector<double>> Percolation::percolation_molloy_reed_criterion(std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links,
                                                                                             unsigned long int number_of_checkpoints) {
    /* "number_of_checkpoints" represents how manny save ticks do you want on your percolation record, if your network
       is too big its better to reduce this A LOT! (you will execute as many outputs as this value) */


    double total = static_cast<double>(list_of_links.size());
    UnionFind uf = UnionFind(this->N);
    std::vector<std::vector<double>> biggest_component(number_of_checkpoints + 1, {0, 1});
    std::vector<unsigned long int> degrees(this->N, 0);

    double sum_of_squared_k = 0;
    double number_of_links_added = 0;
    double molloy_reed_coef = 0;
    double pc = -1;
    unsigned long int biggest_in_pc;

    double progress = 0.0;
    double increment = total / static_cast<double>(number_of_checkpoints);
    double tick_point = increment;

    unsigned int i = 1;
    bool key = true;
    while (!list_of_links.empty()) {
        progress += 1 / total;
        std::pair<unsigned long int, unsigned long int> pair = list_of_links.back();
        list_of_links.pop_back();

        // Add the link
        uf.union_(pair.first, pair.second);
        number_of_links_added += 1;

        // Molloy_reed calculus
        sum_of_squared_k += static_cast<double>(2 * (degrees[pair.first] + degrees[pair.second] + 1));
        degrees[pair.first]++;
        degrees[pair.second]++;
        molloy_reed_coef = sum_of_squared_k / (2 * number_of_links_added);

        // Check if the fraction of nodes added reach the Molloy-Reed criterion limit [<k²>/<k> = 2]
        if ((molloy_reed_coef >= 2) && key) {
            pc = progress;
            biggest_in_pc = uf.get_size_of_max_comp();
            key = false;
        }

        /**************************************************************/
        /*                                                            */
        /*                      Percolation data                      */
        /*                                                            */
        /**************************************************************/
        if ((progress * total >= tick_point) || list_of_links.empty()) {
            if (list_of_links.empty()) {
                biggest_component[i][0] = 1;
            }
            else {
                biggest_component[i][0] = progress;
            }


            biggest_component[i][1] = static_cast<double>(uf.get_size_of_max_comp());
            tick_point += increment;
            i++;
        }
    }

    //Add the critical fraction of added nodes and biggest component in [<k²>/<k> = 2]
    biggest_component.push_back({pc, static_cast<double>(biggest_in_pc)});
    return biggest_component;
}

std::vector<std::vector<double>> Percolation::percolation_molloy_reed(unsigned int number_of_samples) {
//    std::cout <<"[Percolation computation...]"<< std::endl;
    unsigned long int number_of_checkpoints = 9998;
    double pc_mu = 0;
    double bc_mu = 0;
    double mean_l = 0;
    std::vector<std::vector<double>> molloy_reed_p_results(number_of_checkpoints + 2, {0, 0, 0, 0}); // (Fraction of nodes, size of biggest component) {pc_mu, bc_mu, pc_var, bc_var}
    double increment = 1 / static_cast<double>(number_of_samples);
    for (unsigned long int n = 0; n < number_of_samples; n++) {
        progress_bar(increment, n,  number_of_samples);
        std::vector<unsigned long int> degree_list = get_degree_list();
        TopologyBuilder tb = TopologyBuilder(degree_list);
        Graph g = tb.get_g();
        std::thread t (&Percolation::t_geodesical_distance, this, std::ref(mean_l), g.get_adj_matrix());
        //input = list of (Fraction of nodes, size of biggest component)
        std::vector<std::vector<double>> input = percolation_molloy_reed_criterion(g.get_link_list(), number_of_checkpoints);
        for (unsigned int j = 0; j < molloy_reed_p_results.size() ; ++j) {
            pc_mu = molloy_reed_p_results[j][0];
            bc_mu = molloy_reed_p_results[j][1];
            //Mean
            molloy_reed_p_results[j][0] = molloy_reed_p_results[j][0] + ((input[j][0] - molloy_reed_p_results[j][0]) / (n + 1));
            molloy_reed_p_results[j][1] = molloy_reed_p_results[j][1] + ((input[j][1] - molloy_reed_p_results[j][1]) / (n + 1));
            //Var
            molloy_reed_p_results[j][2] = molloy_reed_p_results[j][2] + (input[j][0] - pc_mu) * (input[j][0] - molloy_reed_p_results[j][0]);
            molloy_reed_p_results[j][3] = molloy_reed_p_results[j][3] + (input[j][1] - bc_mu) * (input[j][1] - molloy_reed_p_results[j][1]);
        }

        t.join();

    }
    progress_bar(increment, number_of_samples,  number_of_samples);
    mean_l /= number_of_samples;
    molloy_reed_p_results.push_back({mean_l, -1, -1, -1});
    this->molloy_reed_results = molloy_reed_p_results;
    return molloy_reed_p_results;
}

std::vector<std::vector<double>> Percolation::percolation_configurational(unsigned int number_of_samples) {
//    std::cout <<"[Percolation computation...]"<< std::endl;
    unsigned long int number_of_checkpoints = this->noc;
    double pc_mu = 0;
    double bc_mu = 0;
    double mean_l = 0;
    std::vector<std::vector<double>> molloy_reed_p_results(number_of_checkpoints + 2, {0, 0, 0, 0}); // (Fraction of nodes, size of biggest component) {pc_mu, bc_mu, pc_var, bc_var}
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
        Graph g = tb.get_g();
        std::thread t (&Percolation::t_geodesical_distance, this, std::ref(mean_l), g.get_adj_matrix());
        //input = list of (Fraction of nodes, size of biggest component)
        std::vector<std::vector<double>> input = percolation_molloy_reed_criterion(g.get_link_list(), number_of_checkpoints);
        for (unsigned int j = 0; j < molloy_reed_p_results.size() ; ++j) {
            pc_mu = molloy_reed_p_results[j][0];
            bc_mu = molloy_reed_p_results[j][1];
            //Mean
            molloy_reed_p_results[j][0] = molloy_reed_p_results[j][0] + ((input[j][0] - molloy_reed_p_results[j][0]) / (n + 1));
            molloy_reed_p_results[j][1] = molloy_reed_p_results[j][1] + ((input[j][1] - molloy_reed_p_results[j][1]) / (n + 1));
            //Var
            molloy_reed_p_results[j][2] = molloy_reed_p_results[j][2] + (input[j][0] - pc_mu) * (input[j][0] - molloy_reed_p_results[j][0]);
            molloy_reed_p_results[j][3] = molloy_reed_p_results[j][3] + (input[j][1] - bc_mu) * (input[j][1] - molloy_reed_p_results[j][1]);
        }

        t.join();

    }
    progress_bar(increment, number_of_samples,  number_of_samples);
    mean_l /= number_of_samples;
    molloy_reed_p_results.push_back({mean_l, -1, -1, -1});
    this->molloy_reed_results = molloy_reed_p_results;
    return molloy_reed_p_results;
}

/*********************************************************/
/*                         Write                         */
/*********************************************************/


void Percolation::write_percolation_results(const std::string& filename) {
    std::ofstream myfile;
    myfile.open (filename);
    for (auto value : this->molloy_reed_results) {
        for (unsigned long int i = 0;  i < value.size(); i++ ) {
            if (i < value.size() - 1) myfile << value[i] << ",";
            else {
                myfile << value[i] << std::endl;
            }
        }
    }
    myfile.close();
}



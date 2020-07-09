#include "percolation.h"
#include <functional>

Percolation::Percolation(Distribution * probability_distribution, unsigned long int  N) {
    this->probability_distribution = probability_distribution;
    this->N = N;
}

void Percolation::progress_bar(double progress){
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
    if(progress == 1){
        std::cout << std::endl;
    }
}

double Percolation::get_q(double gamma){
    return (gamma + 1) / gamma;
}

std::vector<unsigned long int> Percolation::get_degree_list(){
    std::vector<unsigned long int> degree_list;
    for(int j = 0; j < this->N; j++){
        unsigned long int val = this->probability_distribution->randint();
        degree_list.push_back(val);
    }
    return degree_list;
}

/*******************************************************************/
/*                         Calculus of <l>                         */
/*******************************************************************/

double Percolation::geodesical_distance_computation(double& mean_l, std::vector<std::vector<unsigned long int>> adj_matrix){
    Bfs bfs = Bfs();
    unsigned long int number_of_samples = 1000;
    number_of_samples = std::min(number_of_samples, this->N);
    mean_l += bfs.avg_geo_dist(number_of_samples,  adj_matrix);
    return mean_l;
}


/******************************************************************/
/*                         Calculus of pc                         */
/******************************************************************/
std::vector<std::vector<double>> Percolation::percolation_molloy_reed_criterion(std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links,
                                                                                             unsigned long int number_of_samples){
    /* "freq_of_reg" represents how manny save ticks do you want on your percolation record, if your network
       is too big its better to reduce this A LOT! (you will execute as many outputs as this value) */

    double total = static_cast<double>(list_of_links.size());
    UnionFind uf = UnionFind(this->N);
    std::vector<std::vector<double>> biggest_component(std::min(number_of_samples, static_cast<unsigned long int>(total)) + 1, {0, 1});
    std::vector<unsigned long int> degrees(this->N, 0);

    double sum_of_squared_k = 0;
    double number_of_links_added = 0;
    double molloy_reed_coef = 0;
    double pc = -1;
    unsigned long int biggest_in_pc;

    double progress = 0.0;
    double tick_point = 1 / static_cast<double>(std::min(number_of_samples, static_cast<unsigned long int>(total)));

    unsigned int i = 1;
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
        if(std::abs(molloy_reed_coef - 2) <= 0.01){
            pc = progress;
            biggest_in_pc = uf.get_size_of_max_comp();
        }

        /**************************************************************/
        /*                                                            */
        /*                      Percolation data                      */
        /*                                                            */
        /**************************************************************/
        if((progress >= tick_point) || ((list_of_links.size()) == 0)){
            biggest_component[i][0] = tick_point;
            biggest_component[i][1] = uf.get_size_of_max_comp();
            tick_point += 1 / static_cast<double>(std::min(number_of_samples, static_cast<unsigned long int>(total)));
            i++;
        }
    }

    //Add the critical fraction of added nodes and biggest component in [<k²>/<k> = 2]
    biggest_component.push_back({pc, biggest_in_pc});
    std::cout<<std::endl;
    return biggest_component;
}

std::vector<std::vector<double>> Percolation::percolation_computation(unsigned int num_rep){
    std::cout <<"[Percolation computation...]"<< std::endl;
    unsigned long int number_of_samples = std::min(this->N, static_cast<unsigned long int>(1000));
    double pc_mu = 0;
    double bc_mu = 0;
    double mean_l = 0;
    std::vector<std::vector<double>> biggest_component(number_of_samples + 2, {0, 0, 0, 0}); // (Fraction of nodes, size of biggest component) {pc_mu, bc_mu, pc_var, bc_var}
    double progress = 0.0;
    for(unsigned long int n = 0; n < num_rep; n++) {
        progress += 1 / static_cast<double>(num_rep);
        std::vector<unsigned long int> degree_list = get_degree_list();
        Topology_builder tb = Topology_builder(degree_list);
        Graph g = tb.get_g();
        geodesical_distance_computation(std::ref(mean_l), g.get_adj_matrix());

        std::cout << "\e[A";
        std::cout<< "[" << n + 1 << "/" << num_rep << "]                  " << std::endl;
        std::vector<std::vector<double>> input = percolation_molloy_reed_criterion(g.get_link_list(), number_of_samples); // (Fraction of nodes, size of biggest component)
        for(unsigned int j = 0; j <= number_of_samples; ++j){
            pc_mu = biggest_component[j][0];
            bc_mu = biggest_component[j][1];
            //Mean
            biggest_component[j][0] = biggest_component[j][0] + ((input[j][0] - biggest_component[j][0]) / (n + 1));
            biggest_component[j][1] = biggest_component[j][1] + ((input[j][1] - biggest_component[j][1]) / (n + 1));
            //Var
            biggest_component[j][2] = biggest_component[j][2] + (input[j][0] - pc_mu) * (input[j][0] - biggest_component[j][0]);
            biggest_component[j][3] = biggest_component[j][3] + (input[j][1] - bc_mu) * (input[j][1] - biggest_component[j][1]);
        }

        if(input[number_of_samples + 1][0] >= 0){
            pc_mu = biggest_component[number_of_samples + 1][0];
            bc_mu = biggest_component[number_of_samples + 1][1];
            //Mean
            biggest_component[number_of_samples + 1][0] = biggest_component[number_of_samples + 1][0] + ((input[number_of_samples + 1][0] - biggest_component[number_of_samples + 1][0]) / (n + 1));
            biggest_component[number_of_samples + 1][1] = biggest_component[number_of_samples + 1][1] + ((input[number_of_samples + 1][1] - biggest_component[number_of_samples + 1][1]) / (n + 1));
            //Var
            biggest_component[number_of_samples + 1][2] = biggest_component[number_of_samples + 1][2] + (input[number_of_samples + 1][0] - pc_mu) * (input[number_of_samples + 1][0] - biggest_component[number_of_samples + 1][0]);
            biggest_component[number_of_samples + 1][3] = biggest_component[number_of_samples + 1][3] + (input[number_of_samples + 1][1] - bc_mu) * (input[number_of_samples + 1][1] - biggest_component[number_of_samples + 1][1]);
        }
        progress_bar(progress);
        // Take carriage 2 alines above to count / progress without messy newlines
        if(n < num_rep -1){
            std::cout << "\e[A";
            std::cout << "\e[A";
        }
    }
    mean_l /= num_rep;
    biggest_component.push_back({mean_l, -1, -1, -1});
return biggest_component;
}

/*********************************************************/
/*                         Write                         */
/*********************************************************/


//void write_random_vector(const std::string& filename, std::vector<unsigned long int> random_vector){
//    std::ofstream myfile;
//    myfile.open (filename);
//    for (unsigned long int i=0; i < random_vector.size(); i++){
//        if(i < random_vector.size() - 1) myfile << random_vector[i] << ",";
//        else {
//            myfile << random_vector[i];
//        }
//    }
//    myfile.close();
//}

//void write_g(const std::string& fn1, const std::string& fn2, Graph graph){
//    std::ofstream myfile;
//    myfile.open (fn1);
//    std::vector<unsigned long int> dist = graph.get_degree_distribution();
//    std::vector<std::pair<unsigned long int, unsigned long int>> link_list = graph.get_link_list();
//    while (!dist.empty()) {
//        unsigned long int deg = dist.back();
//        dist.pop_back();
//        myfile << deg << std::endl;
//    }
//    myfile.close();
//    myfile.open (fn2);
//    while (!link_list.empty()) {
//        std::pair<unsigned long int, unsigned long int> link = link_list.back();
//        link_list.pop_back();
//        myfile << link.first << "," << link.second << std::endl;
//    }
//    myfile.close();
//}

//void write_links(const std::string& filename, std::vector<std::pair<unsigned long int, unsigned long int>> list){
//    std::ofstream myfile;
//    myfile.open (filename);
//    std::cout <<"[Writing graph links...]"<< std::endl;
////    myfile << "Source" << "," << "Target" << std::endl;
//    while (!list.empty()) {
//        std::pair<unsigned long int, unsigned long int> pair = list.back();
//        if(list.size() > 1){
//            myfile << pair.first << "," << pair.second << std::endl;
//        } else{
//            myfile << pair.first << "," << pair.second;
//        }
//        // Remove the link from the list of links
//        list.pop_back();
//    }
//    myfile.close();
//}



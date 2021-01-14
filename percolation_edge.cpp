#include "percolation_edge.h"

Percolation_edge::Percolation_edge() : Percolation("edge", "critical_point") {}

void Percolation_edge::percolate(Graph &G) {
    bool below_critical = true;
    double sum_of_squared_k = 0;
    double number_of_links_added = 0;
    double number_of_links_total;
    double molloy_reed_coef = 0;
    unsigned long int critical_biggest;
    UnionFind uf = UnionFind(G.get_n());
    std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links = G.get_link_list();
    std::vector<unsigned long int> degrees(G.get_n(), 0);
    std::vector<double> biggest_component(1, 1);
    number_of_links_total = list_of_links.size();
    while (!list_of_links.empty()) {
        std::pair<unsigned long int, unsigned long int> pair = list_of_links.back();
        list_of_links.pop_back();

        // Add the link
        uf.union_(pair.first, pair.second);
        number_of_links_added += 1;

        if (below_critical) {
            // Molloy_reed calculus
            sum_of_squared_k += static_cast<double>(2 * (degrees[pair.first] + degrees[pair.second] + 1));
            degrees[pair.first]++;
            degrees[pair.second]++;
            molloy_reed_coef = sum_of_squared_k / (2 * number_of_links_added);
            // Check if the fraction of nodes added reach the Molloy-Reed criterion limit [<k²>/<k> = 2]
            if (molloy_reed_coef >= 2) {
                critical_biggest = uf.get_size_of_max_comp();
                this->other_result = {number_of_links_added/ number_of_links_total, static_cast<double>(critical_biggest)};
                below_critical = false;
            }
        }
        /****************************************************************/
        /*                  Percolation data insert                     */
        /****************************************************************/
        biggest_component.push_back(static_cast<double>(uf.get_size_of_max_comp()));
    }
    this->result = biggest_component;
}

std::vector<double> Percolation_edge::get_result() {
    return this->result;
}

std::vector<double> Percolation_edge::get_other_result() {
    return this->other_result;
}

/***************************************************/
/*                  Deprecated                     */
/***************************************************/


//Percolation_edge::Percolation_edge(unsigned long int number_of_checkpoints) {
//    /* "number_of_checkpoints" represents how manny save ticks do you want on your percolation record, if your network
//       is too big its better to reduce this A LOT! (you will execute as many outputs as this value) */
//    this->noc = number_of_checkpoints;
//}

//void Percolation_edge::percolation_molloy_reed_criterion(Graph & G) {

//    std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links = G.get_link_list();
//    double total = static_cast<double>(list_of_links.size());
//    UnionFind uf = UnionFind(G.get_n());
//    std::vector<std::vector<double>> biggest_component(this->noc + 1, {0, 1});
//    std::vector<unsigned long int> degrees(G.get_n(), 0);

//    double sum_of_squared_k = 0;
//    double number_of_links_added = 0;
//    double molloy_reed_coef = 0;
//    double pc = -1;
//    unsigned long int biggest_in_pc;

//    double progress = 0.0;
//    double increment = total / static_cast<double>(this->noc);
//    double tick_point = increment;

//    unsigned int i = 1;
//    bool key = true;
//    while (!list_of_links.empty()) {
//        progress += 1 / total;
//        std::pair<unsigned long int, unsigned long int> pair = list_of_links.back();
//        list_of_links.pop_back();

//        // Add the link
//        uf.union_(pair.first, pair.second);
//        number_of_links_added += 1;

//        // Molloy_reed calculus
//        sum_of_squared_k += static_cast<double>(2 * (degrees[pair.first] + degrees[pair.second] + 1));
//        degrees[pair.first]++;
//        degrees[pair.second]++;
//        molloy_reed_coef = sum_of_squared_k / (2 * number_of_links_added);

//        // Check if the fraction of nodes added reach the Molloy-Reed criterion limit [<k²>/<k> = 2]
//        if ((molloy_reed_coef >= 2) && key) {
//            pc = progress;
//            biggest_in_pc = uf.get_size_of_max_comp();
//            key = false;
//        }

//        /****************************************************************/
//        /*                  Percolation data insert                     */
//        /****************************************************************/
//        if ((progress * total >= tick_point) || list_of_links.empty()) {
//            if (list_of_links.empty()) {
//                biggest_component[i][0] = 1;
//            }
//            else {
//                biggest_component[i][0] = progress;
//            }


//            biggest_component[i][1] = static_cast<double>(uf.get_size_of_max_comp());
//            tick_point += increment;
//            i++;
//        }
//    }

//    //Add the critical fraction of added nodes and biggest component in [<k²>/<k> = 2]
//    biggest_component.push_back({pc, static_cast<double>(biggest_in_pc)});
//    this->result = biggest_component;
//}

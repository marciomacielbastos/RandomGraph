#include "percolation_edge.h"

Percolation_edge::Percolation_edge() {}

void Percolation_edge::percolate(Graph &G) {
    bool below_critical = true;
    double sum_of_squared_k = 0;
    double number_of_links_added = 0;
    double number_of_links_total;
    double molloy_reed_coef = 0;
    unsigned long int critical_biggest;
    UnionFind uf = UnionFind(G.get_n());
    std::vector<std::pair<unsigned long int, unsigned long int>> list_of_links = G.get_links_vector();
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
                critical_biggest = uf.get_maximal_component_size();
                below_critical = false;
            }
        }
        /****************************************************************/
        /*                  Percolation data insert                     */
        /****************************************************************/
        biggest_component.push_back(static_cast<double>(uf.get_maximal_component_size()));
    }
    this->result = biggest_component;
}

std::vector<double> Percolation_edge::get_result() {
    return this->result;
}

#include "percolation.h"

Percolation::Percolation(unsigned long int  N, unsigned long int lambda, unsigned long int gamma, unsigned long int xmin){
    this->N = N;
    this->lambda = lambda;
    this->gamma = gamma;
    this->xmin = xmin;
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
    unsigned long int k;
    int count = 0;
    // c = N * <k>
    unsigned long int c =0;
    unsigned long int a = 2 * (this->N - 1);
    unsigned long int b = this->N * (this->N -1);
    std::vector<unsigned long int> degree_list;
    q_Exponential q = q_Exponential(this->lambda, get_q(this->gamma), this->xmin, this->N);
    while(c < a || c > b){
        degree_list.clear();
        c = 0;
        for(unsigned long int i=0; i< this->N; i++){
            k = q.randint();
            c += k;
            degree_list.push_back(k);
        }
        count++;
        if(count >= 100){
            degree_list.clear();
            break;
        }
    }
    return degree_list;
}

std::vector<std::pair<unsigned long int, unsigned long int>> Percolation::get_link_list(){
    return this->g.get_link_list();
}

/*************************************************************************/
/*                         To write <l> graphs                           */
/*************************************************************************/

UnionFind Percolation::mount_geodesical_stats(Rede rd){
    std::vector<std::pair<unsigned long int, unsigned long int>> list = rd.get_list_of_links();
    UnionFind uf = UnionFind(rd.get_number_of_nodes());
    while (list.size() > 0) {
        std::pair<unsigned long int, unsigned long int> pair = list[list.size() - 1];
        // Union the nodes of this link
        uf.union_(pair.first, pair.second);
        // Remove the link from the list of links
        list.pop_back();
    }
    return uf;
}

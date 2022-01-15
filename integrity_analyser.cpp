#include "integrity_analyser.h"

Integrity_analyser::Integrity_analyser(unsigned long int N) {
    std::vector<unsigned long int> sizes(N, 1);
    this->sizes = sizes;
    this->N = N;
    UnionFind uf(this->N);
    this->uf = uf;

}

void Integrity_analyser::union_(unsigned long int v, unsigned long int w) {
    unsigned long int head_v = this->uf.find(v);
    unsigned long int head_w = this->uf.find(w);
    if (head_v != head_w) {
        if (uf.get_size_of_component(head_v) < uf.get_size_of_component(head_w)) {
            unsigned long int temp = head_v;
            head_v = head_w;
            head_w = temp;
        }
        this->sizes[head_v] = 0;
        this->sizes[head_w] = uf.get_size_of_component(head_v) + uf.get_size_of_component(head_w);
        uf.union_(v, w);
    }
}

void Integrity_analyser::flush() {
    for (unsigned long int i; i < this->sizes.size(); i++) {
        this->sizes[i] = 1;
    }
    UnionFind uf(this->N);
    this->uf = uf;
}

unsigned long int Integrity_analyser::read_file(std::string filename, char delimiter) {
    std::string line;
    std::ifstream myfile (filename);
    std::string item;
    unsigned long int edge[2];
    if (myfile.is_open()) {
      while ( std::getline (myfile,line) ) {
        std::istringstream iss(line);
        int i = 0;
        while (std::getline(iss, item, delimiter)) {
              edge[i] = std::stoul(item);
              i++;
        }
        union_(edge[0], edge[1]);
      }
      myfile.close();
      return uf.get_maximal_component_size();
    }

    else {
        std::cout << "Unable to open file: " << filename << std::endl;
        return 0;
    }
}

unsigned long int Integrity_analyser::get_max() {
    unsigned long int max = 0;
    for (unsigned long int i=0; i < this->N; i++) {
        if (max < this->sizes[i]) {
            max = this->sizes[i];
        }
    }
    return max;
}

#ifndef INTEGRITY_ANALYSER_H
#define INTEGRITY_ANALYSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <unionfind.h>

class Integrity_analyser {
private:
    unsigned long int N;
    UnionFind uf;
    std::vector<unsigned long int> sizes;

    void union_(unsigned long int v, unsigned long int w);

public:
    Integrity_analyser(unsigned long int N);
    unsigned long int read_file(std::string filename, char delimiter);
    unsigned long int get_max();
    void flush();
};

#endif // INTEGRITY_ANALYSER_H

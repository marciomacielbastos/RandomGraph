#ifndef PERCOLATION_H
#define PERCOLATION_H
#include <graph.h>
#include <vector>
#include <fstream>
#include <sstream>

class Percolation {
protected:
    std::string name;
    std::string other;
    std::vector<double> result;
    std::vector<double> other_result;

public:
   virtual std::vector<double> get_result() = 0;
   virtual std::vector<double> get_other_result() = 0;
   virtual void percolate(Graph &G) = 0;
   Percolation (std::string name) {
        this->name = name;
   }

   Percolation (std::string name, std::string other) {
        this->name = name;
        this->other = other;
   }
   std::string get_name() {
        return this->name;
   }
   std::string get_other() {
        return this->other;
   }
};

#endif // PERCOLATION_H

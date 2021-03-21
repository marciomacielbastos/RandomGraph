#ifndef PERCOLATION_H
#define PERCOLATION_H
#include <fstream>
#include <vector>
#include <sstream>

#include <graph.h>
#include <unionfind.h>

class Percolation {
protected:
    std::vector<double> result;

public:
   virtual std::vector<double> get_result() = 0;
   virtual void percolate(Graph &G) = 0;
   void flush(){
       unsigned int total = result.size();
       for (unsigned int var = 0; var < total; ++var) {
           result.pop_back();
       }
   }
};

#endif // PERCOLATION_H

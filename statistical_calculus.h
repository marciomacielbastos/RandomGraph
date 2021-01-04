#ifndef STATISTICAL_CALCULUS_H
#define STATISTICAL_CALCULUS_H
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <percolation.h>
#include <graph.h>


class Statistical_calculus {
private:
    double increment;
    std::vector<Percolation*> percolations;
    std::vector<std::vector<std::vector<double>>> results;
    std::vector<std::vector<double>> other_results;
    void progress_bar(unsigned long int i);
    std::string get_filename(std::string folder, double gamma, double lambda, unsigned long kmin, unsigned long n, unsigned long int i);
    void percolate(Percolation *p, Graph &G);
    void mean_std_result(unsigned long int i,
                         std::vector<std::vector<double>>& mean_results,
                         std::vector<double> observed_values);
    void mean_std_other_results(unsigned long int i,
                                std::vector<double>& mean_results,
                                std::vector<double> observed_values);

public:
    Statistical_calculus();
    void add_percolation(Percolation *p);
    void calc(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned long int number_of_samples, std::string folder);
    std::vector<std::vector<std::vector<double>>> get_results();
    std::vector<std::vector<double>> get_other_results();
};

#endif // STATISTICAL_CALCULUS_H

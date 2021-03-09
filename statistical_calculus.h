#ifndef STATISTICAL_CALCULUS_H
#define STATISTICAL_CALCULUS_H
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <percolation.h>
#include <thread>
#include <graph.h>


class Statistical_calculus {
private:
    double increment;
    std::vector<Percolation*> percolations;
    std::vector<std::vector<double>> realization;
    std::vector<std::vector<double>> other_realization;
    std::vector<std::vector<std::vector<double>>> results;
    std::vector<std::vector<std::vector<double>>> other_results;


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
    void progress_bar(unsigned long int i);
    void progress_bar(unsigned long int i, double increment);
    void add_percolation(Percolation *p);
    void calc_statistics(double gamma, double lambda, unsigned long int kmin, unsigned long int N, unsigned long int number_of_samples, std::string folder);
    void calc(std::string filepath, unsigned long int N);
    std::vector<std::vector<std::vector<double>>> get_results();
    std::vector<std::vector<std::vector<double>>> get_other_results();
    std::vector<std::vector<double>> get_realization();
    std::vector<std::vector<double>> get_other_realization();
    std::string get_percolation_name(unsigned int i);
    std::string get_percolation_other(unsigned int i);
};

#endif // STATISTICAL_CALCULUS_H

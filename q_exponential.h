#ifndef Q_EXPONENTIAL_H
#define Q_EXPONENTIAL_H
#include "distribution.h"
#include "quick_search.h"
#include "uniform.h"
#include <vector>
#include <cmath>

class qExponential : public Distribution {
private:
    double normalization_constant;
    double lambda;
    double q;
    unsigned long int xmin = 0;
    unsigned long int xmax;
    std::vector<double> cdf;

public:
    qExponential();
    qExponential(double lambda, double q, unsigned long int xmin, unsigned long int N);
    double coefficient(unsigned long int x);
    double probability (unsigned long int lower, unsigned long int upper);
    double norm_const();
    double eq(double x);
    double pdf(unsigned long int x);
    double pmf(unsigned long int x);
    void build_dist();
    unsigned long int search_inverse_CDF(double p);
    unsigned long int natural_cutoff(unsigned long int N);
    unsigned long int randint();
};

#endif // Q_EXPONENTIAL_H

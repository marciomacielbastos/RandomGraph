#ifndef Q_EXPONENTIAL_H
#define Q_EXPONENTIAL_H
#include "distribution.h"
#include "quick_search.h"
#include "uniform.h"
#include <vector>
#include <cmath>

class q_Exponential : public Distribution{
private:
    double lambda;
    double q;
    unsigned long int xmin = 0;
    unsigned long int xmax;
    std::vector<double> cdf;

public:
    q_Exponential();
    q_Exponential(double lambda, double q, unsigned long int xmin, unsigned long int xmax);
    double eq(double x);
    double pdf(unsigned long int x);
    void build_dist();
    unsigned long int search_inverse_CDF(double p);
    unsigned long int randint();
};

#endif // Q_EXPONENTIAL_H

#ifndef POWER_LAW_H
#define POWER_LAW_H
#include "distribution.h"
#include "quick_search.h"
#include "uniform.h"
#include <vector>
#include <cmath>

class Power_law: public Distribution {
private:
    double gamma;
    unsigned long int xmin = 0;
    unsigned long int xmax;
    std::vector<double> cdf;

public:
    Power_law();
    Power_law(double gamma, unsigned long int xmin, unsigned long int xmax);
    double pdf(unsigned long int x);
    void build_dist();
    unsigned long int search_inverse_CDF(double p);
    unsigned long int randint();
};

#endif // POWER_LAW_H
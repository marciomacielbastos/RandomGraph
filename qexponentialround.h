#ifndef QEXPONENTIALROUND_H
#define QEXPONENTIALROUND_H
#include "distribution.h"
#include "quick_search.h"
#include "uniform.h"
#include <vector>
#include <cmath>

class qExponentialRound : public Distribution {
private:
    double lambda;
    double q;
    double C;
    unsigned long int xmin;
    unsigned long int xmax;
    double F_min;
    double F_max;
    double F_min_g;
    double F_max_g;
    double F(double x);

public:
    qExponentialRound(double lambda, double q, unsigned long int xmin, unsigned long int N);
    unsigned long int calc_x (double u);
    unsigned long int randint();
};
#endif // QEXPONENTIALROUND_H

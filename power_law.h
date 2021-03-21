#ifndef POWER_LAW_H
#define POWER_LAW_H
#include "probability_distribution.h"
#include "quick_search.h"
#include "uniform.h"
#include <vector>
#include <cmath>

class PowerLaw : public Probability_distribution {
private:
    double gamma;
    double C;
    unsigned long int xmin;
    unsigned long int xmax;
    double F_min;
    double F_max;

public:
    PowerLaw(double gamma, unsigned long int xmin, unsigned long int N);
    unsigned long int calc_x (double u);
    unsigned long int randint();
};

#endif // POWER_LAW_H

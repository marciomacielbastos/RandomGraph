#ifndef POISSON_H
#define POISSON_H
#include "probability_distribution.h"
#include "uniform.h"
#include <cmath>

class Poisson : public Probability_distribution {
private:
    double lambda;
public:
    Poisson(double lambda);
    unsigned long int randint();
};
#endif // POISSON_H

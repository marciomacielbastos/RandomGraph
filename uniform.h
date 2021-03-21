#ifndef RANDINT_H
#define RANDINT_H
#include <probability_distribution.h>
#include <algorithm>

class Uniform : public Probability_distribution {
public:
    Uniform();
    unsigned long int randint(unsigned long int xmin, unsigned long int max);
    unsigned long int randint(unsigned long int max);
    unsigned long int randint( );
    double random();
};

#endif // RANDINT_H

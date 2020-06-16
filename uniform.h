#ifndef RANDINT_H
#define RANDINT_H
#include <distribution.h>

class Uniform : public Distribution{
public:
    Uniform();
    unsigned long int randint(unsigned long int xmin, unsigned long int max);
    unsigned long int randint(unsigned long int max);
    double random();
};

#endif // RANDINT_H

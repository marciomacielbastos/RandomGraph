#include "poisson.h"

Poisson::Poisson(double lambda) {
    this->lambda = lambda;
}

//Source: https://hpaulkeeler.com/simulating-poisson-random-variables-direct-method/
unsigned long int Poisson::randint() {
    double exp_lambda = std::exp(-this->lambda);
    Uniform u;
    double randUni = u.random(); //uniform variable
    double prodUni =  randUni; //product of uniform variables
    long int randPoisson = -1; //Poisson variable
    while (prodUni > exp_lambda) {
        randUni = u.random();
        prodUni *= randUni;
        randPoisson++;
    }
    if (randPoisson < 0) {
        randPoisson = 0;
    }
    return static_cast<unsigned long int>(randPoisson);
}

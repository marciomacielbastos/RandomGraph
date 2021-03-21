#include "power_law.h"


PowerLaw::PowerLaw(double gamma, unsigned long int xmin, unsigned long int N):gamma(gamma), xmin(xmin) {
    this->xmax = N - 1;
    double exp = 1 - this->gamma;
    this->F_min = std::pow(static_cast<double>(this->xmin), exp);
    this->F_max = std::pow(static_cast<double>(this->xmax), exp);
    this->C = 1 / (this->F_max - this->F_min);
}

unsigned long int PowerLaw::calc_x(double u) {
    unsigned long int x;
    double exponent = 1 / (1 - this->gamma);
    double real_x  = this->F_min + (u / this->C);
    real_x = std::pow(real_x, exponent);
    x = std::round(real_x);
    return x;
}

unsigned long int PowerLaw::randint(){
    Uniform u = Uniform();
    double var = u.random();
    return calc_x(var);
}

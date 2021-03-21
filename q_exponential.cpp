#include "q_exponential.h"

qExponential::qExponential(double lambda, double q, unsigned long int xmin, unsigned long int N):lambda(lambda), q(q), xmin(xmin) {
    this->xmax = N - 1;
    this->F_min = F(static_cast<double>(this->xmin));
    this-> F_max = F(static_cast<double>(this->xmax));
    double gamma = 1 / (1 - this->q);
    this->F_min_g = std::pow(this->F_min, gamma);
    this->F_max_g = std::pow(this->F_max, gamma);
    double partial_result = (this->F_min * this->F_min_g) - (this->F_max * this->F_max_g);
    this->C = 1 / partial_result;
}

double qExponential::F(double x) {
    double result = 1 - (1- q) * this->lambda * x;
    return result;
}

unsigned long int qExponential::calc_x(double u) {
    unsigned long int x;
    double exponent = (1 - q) / (2 - q);
    double real_x  = (this->F_min * this->F_min_g) - (u / this->C);
    real_x = std::pow(real_x, exponent) - 1;
    real_x /= (q - 1) * this->lambda;
    x = std::round(real_x);
    return x;
}

unsigned long int qExponential::randint(){
    Uniform u = Uniform();
    double var = u.random();
    return calc_x(var);
}

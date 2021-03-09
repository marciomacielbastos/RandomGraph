#include "q_exponential.h"

qExponential::qExponential(){
    this->normalization_constant = 1;
    this->xmin = 0;
}

qExponential::qExponential(double lambda, double q, unsigned long int xmin, unsigned long int N):lambda(lambda), q(q), xmin(xmin) {
    this->xmax = N - 1;
//    this->xmax = natural_cutoff(lambda, q, kmin, N);
    this->normalization_constant = norm_const();
    this->build_dist();
}

double qExponential::coefficient(unsigned long int x) {
    double coef = 1 - ((1 - this->q) * this->lambda * static_cast<double>(x));
    return coef;
}

double qExponential::probability(unsigned long int lower, unsigned long int upper) {
    if (lower > upper) {
        unsigned long int temp = lower;
        lower = upper;
        upper = temp;
    }
    double exponent =  (1 / (1 - this->q)) + 1;
    double c_xmin = coefficient(lower);
    double c_xmax = coefficient(upper);
    c_xmin = std::pow(c_xmin, exponent);
    c_xmax = std::pow(c_xmax, exponent);
    return (c_xmin - c_xmax);
}

double qExponential::norm_const() {
    double val =  probability(this->xmin, this->xmax);
    return 1 / (val);
}

unsigned long int qExponential::natural_cutoff(unsigned long int N) {
    double exponent =  (q - 1) / (2 - this->q);
    double coef = - coefficient(this->xmin);
    double max = ((coef * std::pow(static_cast<double>(N), exponent)) + 1) / ((1 - q) * lambda);
    return static_cast<unsigned long int>(max);
}

double qExponential::eq(double x){
    return std::pow((1 + (1 - this->q) * x), (1 / (1-this->q)));
}

double qExponential::pdf(unsigned long int x){
    return  (this->normalization_constant) * (2 - this->q) * this->lambda * eq((-this->lambda) * static_cast<double>(x));
}

double qExponential::pmf(unsigned long int x) {
    if (x < this->xmin) x = this->xmin;
    double val = probability(x, x + 1);
    return this->normalization_constant * val;
}

void qExponential::build_dist(){
    double sum = 0;
//    for(unsigned long int i = this->xmin; i <= this->xmax; i++){
//        double val = pdf(i);
//        sum += val;
//        this->cdf.push_back(sum);
//    }

//    for(unsigned long int i = 0; i < this->cdf.size(); i++){
//        this->cdf[i] = this->cdf[i] / sum;
//    }
    double val;
    for (unsigned long int i = this->xmin; i < this->xmax; i++) {
         val = pmf(i);
         sum += val;
         this->cdf.push_back(sum);
    }
}

unsigned long int qExponential::search_inverse_CDF(double p){
    QuickSearch quick;
    unsigned long int position = quick.search(this->cdf, p, 0, this->cdf.size());
    return position;
}

unsigned long int qExponential::randint(){
    Uniform u = Uniform();
    return this->xmin + search_inverse_CDF(u.random());
}

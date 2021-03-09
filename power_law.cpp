#include "power_law.h"

Power_law::Power_law() {
    this->normalization_constant = 1;
}

Power_law::Power_law(double gamma, unsigned long int xmin, unsigned long int N):gamma(gamma), xmin(xmin) {
    this->xmax = N - 1;
    this->normalization_constant = norm_const();
    this->build_dist();
}


double Power_law::pdf(unsigned long int x) {
    return this->normalization_constant * std::pow((1/ static_cast<double>(x)), this->gamma);
}

double Power_law::pmf(unsigned long int x) {
    if (x < this->xmin) x = this->xmin;
    double val = probability(x, x + 1);
    return this->normalization_constant * val;
}

double Power_law::norm_const() {
    double C = probability(this->xmin, this->xmax);
    return 1 / C;
}

double Power_law::probability(unsigned long lower, unsigned long upper) {
    if (lower > upper) {
        unsigned long int temp = lower;
        lower = upper;
        upper = temp;
    }
    double exp = 1 - this->gamma;
    double x_min = std::pow(static_cast<double>(lower), exp);
    double x_max = std::pow(static_cast<double>(upper), exp);
    double C = (x_max - x_min) / exp;
    return C;
}

void Power_law::build_dist() {
    double sum = 0;
//    for (unsigned long int i = 1; i < this->xmax; i++) {
//        double val = pdf(i);
//        sum += val;
//        this->cdf.push_back(sum);
//    }

//    for (unsigned long int i = 0; i < this->cdf.size(); i++) {
//        this->cdf[i] = this->cdf[i] / sum;
//    }
    double val;
    for (unsigned long int i = this->xmin; i < this->xmax; i++) {
         val = pmf(i);
         sum += val;
         this->cdf.push_back(sum);
    }
}

unsigned long int Power_law::search_inverse_CDF(double p) {
    QuickSearch quick;
    unsigned long int position = quick.search(this->cdf, p, 0, this->cdf.size());
    return position;
}

unsigned long int Power_law::randint(){
    Uniform u = Uniform();
    return this->xmin + search_inverse_CDF(u.random());
}

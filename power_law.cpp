#include "power_law.h"

Power_law::Power_law() {

}

Power_law::Power_law(double gamma, unsigned long int xmin, unsigned long int xmax):gamma(gamma), xmin(xmin), xmax(xmax) {
    this->build_dist();
}


double Power_law::pdf(unsigned long int x) {
    return std::pow((1/ static_cast<double>(x)), this->gamma);
}

void Power_law::build_dist() {
    double sum = 0;
    for (unsigned long int i = 0; i < this->xmax; i++) {
        double val = pdf(i);
        sum += val;
        this->cdf.push_back(val);
    }

    this->cdf[0] = this->cdf[0] / sum;
    for (unsigned long int i = 1; i < this->cdf.size(); i++) {
        this->cdf[i] = this->cdf[i - 1] + (this->cdf[i] / sum);
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

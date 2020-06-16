#include "q_exponential.h"

q_Exponential::q_Exponential(){

}

q_Exponential::q_Exponential(double lambda, double q, unsigned long int xmin, unsigned long int xmax):lambda(lambda), q(q), xmin(xmin), xmax(xmax){
    if (this->q < 1) {
        double factor = 1 / (lambda * (1 - this->q));
        this->xmax = static_cast<unsigned long int>(std::fmin(this->xmax - 1, static_cast<unsigned long int>(factor)));
    }
    this->build_dist();
}


double q_Exponential::eq(double x){
    return std::pow((1 + (1 - this->q) * x), (1 / (1-this->q)));
}

double q_Exponential::pdf(unsigned long int x){
    return (2 - this->q) * this->lambda * eq((-this->lambda) * static_cast<double>(x));
}

void q_Exponential::build_dist(){
    double sum = 0;
    for(unsigned long int i = 0; i < this->xmax; i++){
        double val = pdf(i);
        sum += val;
        this->cdf.push_back(val);
    }

    this->cdf[0] = this->cdf[0] / sum;
    for(unsigned long int i = 1; i < this->cdf.size(); i++){
        this->cdf[i] = this->cdf[i - 1] + (this->cdf[i] / sum);
    }
}

unsigned long int q_Exponential::search_inverse_CDF(double p){
    Quick_search quick;
    unsigned long int position = quick.search(this->cdf, p, 0, this->cdf.size());
    return position;
}

unsigned long int q_Exponential::randint(){
    Uniform u = Uniform();
    return this->xmin + search_inverse_CDF(u.random());
}

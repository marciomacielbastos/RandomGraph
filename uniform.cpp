#include "uniform.h"

Uniform::Uniform(){

}

unsigned long int Uniform::randint(unsigned long xmin, unsigned long xmax){
    xmax = std::max(xmin, xmax - 1);
    if(xmin == xmax){
        return xmin;
    }
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(xmin, xmax);
    return dis(gen);
}

unsigned long int Uniform::randint(unsigned long int xmax){
    return Uniform::randint(0, xmax);
}

unsigned long int Uniform::randint(){
    return randint(0, 2);
}

double Uniform::random(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0,1.0);
    return dis(gen);
}

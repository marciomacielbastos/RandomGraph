#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include <random>
#include <algorithm>    // std::min


class Probability_distribution {
public:
   // pure virtual function
//   virtual unsigned long int randint(unsigned long int xmin, unsigned long int max) = 0;
//   virtual unsigned long int randint(unsigned long int max) = 0;
   virtual unsigned long int randint() = 0;
};

#endif // DISTRIBUTION_H

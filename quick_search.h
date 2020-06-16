#ifndef SEARCH_H
#define SEARCH_H
#include <vector>


class Quick_search
{
public:
    Quick_search();
    static unsigned long int search(const std::vector<double>& list, double value, unsigned long int j_min, unsigned long int j_max);
};

#endif // SEARCH_H

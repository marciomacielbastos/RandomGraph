#include "quick_search.h"


QuickSearch::QuickSearch(){

}

unsigned long int QuickSearch::search(const std::vector<double>& list, double value, unsigned long int j_min, unsigned long int j_max){
    unsigned long int mid =  j_min + static_cast<unsigned long int>((double(j_max) - double(j_min)) / 2);
    if (mid == j_min){
        return mid;
    }
    else if (value < list[mid]){
        return search(list, value, j_min, mid);
    }
    else if (value > list[mid]){
        return search(list, value, mid, j_max);
    }
}

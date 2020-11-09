#ifndef HEAP_H
#define HEAP_H
#include <vector>



class Heap
{
private:
    std::vector<unsigned long int> degrees;
    std::vector<unsigned long int> positions;
    std::vector<unsigned long int> heap;

public:
    Heap(std::vector<unsigned long int> & degrees);
    void swap(unsigned long int i, unsigned long j);
    void up(unsigned long int i);
    void down(unsigned long int i);
    unsigned long int pop();
    void sort();
    void update(unsigned long int i);
    unsigned long int size();
};

#endif // HEAP_H

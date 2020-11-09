#include "heap.h"

Heap::Heap(std::vector<unsigned long int> & degrees) {
    this->degrees = degrees;
    for (unsigned long int i = 0; i < degrees.size(); ++i) {
        this->positions.push_back(i);
        this->heap.push_back(i);
    }
}

void Heap::swap(unsigned long i, unsigned long j){
    unsigned long int temp = this->heap[i];
    this->heap[i] = this->heap[j];
    this->heap[j] = temp;
    this->positions[this->heap[i]] = i;
    this->positions[this->heap[j]] = j;
}

void Heap::up(unsigned long i){
    unsigned long int j = (i + 1) / 2;
    if (j >= 1) {
        if (this->id_degree[this->heap[i]] < this->id_degree[this->heap[j - 1]]) {
            swap(i, j);
            up(j);
        }
    }
}

void Heap::down(unsigned long int i){
    unsigned long int j = 2 * (i + 1);
    unsigned long int n = this->id_degree.size();
    if (j <= n) {
        if (j < n) {
            if (this->id_degree[this->heap[j]] < this->id_degree[this->heap[j - 1]]) {
                j++;
            }
        }
        if (this->id_degree[this->heap[i]] > this->id_degree[this->heap[j - 1]]) {
            swap(i, j);
            down(j - 1);
        }
    }
}

void Heap::sort() {
    unsigned long int h = this->heap.size() / 2;
    for (unsigned long int i = h; i > 1; --i) {
        down(i - 1);
    }
}

void Heap::update(unsigned long i){
    if(this->degrees[i] > 0) {
        this->degrees[i]--;
        up(this->heap[this->positions[i]]);
    }
}

unsigned long int Heap::pop(){
    swap(0, this->heap.size() - 1);
    this->heap.pop();
    down(0);
}

unsigned long int Heap::size(){
    return this->heap.size();
}

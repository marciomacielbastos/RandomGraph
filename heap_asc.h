#ifndef HEAP_ASC_H
#define HEAP_ASC_H
#include <vector>


template <class T>
class Heap_asc {
private:
    std::vector<T> values;
    std::vector<unsigned long int> positions;
    std::vector<unsigned long int> heap;

public:
    Heap_asc(std::vector <T> & values);
    void swap(unsigned long int i, unsigned long j);
    void up(unsigned long int i);
    void down(unsigned long int i);
    unsigned long int pop();
    void sort();
    void update(unsigned long int i);
    unsigned long int size();
    T get_root_value();
    bool is_empty();
};

/******************************************************************************/
/*                               IMPLEMENTATION                               */
/******************************************************************************/

template <typename T>
Heap_asc<T>::Heap_asc(std::vector<T> & values) {
    this->values = values;
    for (unsigned long int i = 0; i < values.size(); ++i) {
        this->positions.push_back(i);
        this->heap.push_back(i);
    }
    sort();
}
template <typename T>
void Heap_asc<T>::swap(unsigned long i, unsigned long j){
    unsigned long int temp = this->heap[i];
    this->heap[i] = this->heap[j];
    this->heap[j] = temp;
    this->positions[this->heap[i]] = i;
    this->positions[this->heap[j]] = j;
}
template <typename T>
void Heap_asc<T>::up(unsigned long i){
    unsigned long int j = (i + 1) / 2;
    if (j >= 1) {
        if (this->values[this->heap[i]] < this->values[this->heap[j - 1]]) {
            swap(i, j - 1);
            up(j);
        }
    }
}
template <typename T>
void Heap_asc<T>::down(unsigned long int i){
    unsigned long int j = 2 * (i + 1);
    unsigned long int n = this->heap.size();
    if (j <= n) {
        if (j < n) {
            if (this->values[this->heap[j]] < this->values[this->heap[j - 1]]) {
                j++;
            }
        }
        if (this->values[this->heap[i]] > this->values[this->heap[j - 1]]) {
            swap(i, j - 1);
            down(j - 1);
        }
    }
}
template <typename T>
void Heap_asc<T>::sort() {
    unsigned long int h = this->heap.size() / 2;
    for (unsigned long int i = h; i >= 1; --i) {
        down(i - 1);
    }
}
template <typename T>
void Heap_asc<T>::update(unsigned long i){
    if(this->values[i] > 0 && this->positions[i] < size()) {
        this->values[i]--;
        up(this->positions[i]);
    }
}
template <typename T>
unsigned long int Heap_asc<T>::pop(){
    unsigned long int root = this->heap[0];
    this->values[this->heap[0]] = 0;
    swap(0, this->heap.size() - 1);
    this->heap.pop_back();
    down(0);
    return root;
}
template <typename T>
unsigned long int Heap_asc<T>::size(){
    return this->heap.size();
}
template <typename T>
T Heap_asc<T>::get_root_value(){
    return this->values[this->heap[0]];
}
template <typename T>
bool Heap_asc<T>::is_empty(){
    return this->heap.empty();
}


#endif // HEAP_ASC_H

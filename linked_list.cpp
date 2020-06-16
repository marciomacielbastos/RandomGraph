#include "linked_list.h"

Linked_list::Linked_list(){

}

Linked_list::Linked_list(std::vector<unsigned long int> degree_list){
    this->binders = degree_list;
    for(std::vector<unsigned long int>::iterator it = this->binders.begin(); it != this->binders.end() ; ++it){
        this->addresses.push_back(it);
    }
}

void Linked_list::link(unsigned long v, unsigned long w){
    this->binders[v] += this->binders[w];
    this->binders[w] = this->binders[this->binders.back()];
    this->binders.pop_back();
}


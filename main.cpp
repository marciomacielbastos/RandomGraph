#include <QCoreApplication>
#include <iostream>

#include <rb_tree.h>


//This code is contributed by rathbhupendra

int main(int argc, char** argv) {
    Rb_tree *rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt = NULL;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;
    rbt =  new Rb_tree();
    for (unsigned long int i=0; i < 5000000; i++) {
        rbt->insert(i);
    }
    delete rbt;

}


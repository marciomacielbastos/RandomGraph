#ifndef RB_TREE_H
#define RB_TREE_H
#include <vector>

// data structure that represents a node in the tree
struct Node {
    unsigned long int key; // holds the key
    Node *parent; // pointer to the parent
    Node *left; // pointer to left child
    Node *right; // pointer to right child
    Node *next;
    bool color; // 1 -> Red, 0 -> Black
};
typedef Node *NodePtr;

class Rb_tree {
private:
    NodePtr root;
    NodePtr tail;
    NodePtr NIL;
    unsigned long int tree_size = 0;
    void leftRotate(NodePtr x);
    void rightRotate(NodePtr x);
    void rebalance_insert(NodePtr node);
    void rebalance_delete(NodePtr node);
    NodePtr search_tree(NodePtr node, unsigned long int key);
    void transplant(NodePtr u, NodePtr v);

public:
    Rb_tree();
//    ~Rb_tree();
    bool is_present(unsigned long int k);
    NodePtr search(unsigned long int key);
    bool insert(unsigned long int key);
    void _delete(unsigned long int key);
    unsigned long int size();
    NodePtr minimum(NodePtr node);
    NodePtr maximum(NodePtr node);
    bool empty();
};

#endif // RB_TREE_H
// Tutorial URL: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/

#ifndef RB_TREE_H
#define RB_TREE_H
#include <map>

// data structure that represents a node in the tree
//struct Node {
//    unsigned long int key; // holds the key
//    Node *parent; // pointer to the parent
//    Node *left; // pointer to left child
//    Node *right; // pointer to right child
//    bool color; // 1 -> Red, 0 -> Black
//};

struct Node {
    unsigned long int key; // holds the key
    Node *parent; // pointer to the parent
    Node *left; // pointer to left child
    Node *right; // pointer to right child
    bool color; // 1 -> Red, 0 -> Black
};
typedef Node *NodePtr;

class Rb_tree {
private:
    NodePtr root;
    NodePtr NIL;
    unsigned long int tree_size;
    void leftRotate(NodePtr x);
    void rightRotate(NodePtr x);
    void rebalance_insert(NodePtr node);
    void rebalance_delete(NodePtr node);
    void transplant(NodePtr u, NodePtr v);
    void destroy_recursive(NodePtr node);
    NodePtr search_tree(NodePtr node, unsigned long int key);
    NodePtr build_node(unsigned long int key);


public:
    Rb_tree();
    ~Rb_tree();

    bool empty();
    bool insert(unsigned long int key);
    bool is_present(unsigned long int k);
    NodePtr maximum(NodePtr node);
    NodePtr minimum(NodePtr node);
    NodePtr search(unsigned long int key);
    unsigned long int size();
    void delete_node(unsigned long int key);
};

#endif // RB_TREE_H
// Tutorial URL: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/

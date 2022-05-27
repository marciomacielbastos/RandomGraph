#ifndef RB_TREE_H
#define RB_TREE_H
#include <map>
#include <queue>
#include <vector>

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
    void destroy_recursive(NodePtr node);
    void leftRotate(NodePtr x);
    void rightRotate(NodePtr x);
    void rebalance_insert(NodePtr node);
    void rebalance_delete(NodePtr node);
    void swap_keys(NodePtr v, NodePtr u);
    void transplant(NodePtr u, NodePtr v);  
    NodePtr build_node(unsigned long int key);
    NodePtr replacement(NodePtr node);
    NodePtr search_tree(NodePtr node, unsigned long int key);   
    std::queue<NodePtr> queue;


public:
    Rb_tree();
    ~Rb_tree();

    bool empty();
    bool insert(unsigned long int key);
    bool is_present(unsigned long int k);
    NodePtr maximum(NodePtr node);
    NodePtr minimum(NodePtr node);
    NodePtr search(unsigned long int key);
    NodePtr next();
    unsigned long int size();
    void delete_key(unsigned long int key);
    void delete_node(NodePtr v);
    void iterate();
    std::vector<unsigned long int> bfs();


};

#endif // RB_TREE_H
// Tutorial URL: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/

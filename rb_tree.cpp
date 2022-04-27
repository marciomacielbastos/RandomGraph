#include "rb_tree.h"

Rb_tree::Rb_tree() {
    this->NIL = new Node;
    this->NIL->parent = nullptr;
    this->NIL->key = 0;
    this->NIL->left = nullptr;
    this->NIL->right = nullptr;
    this->NIL->color = 0; // New node must be red
    this->root = this->NIL;
    this->tree_size = 0;
}

void Rb_tree::destroy_recursive(NodePtr node)
{
    if (node != this->NIL and node)
    {
        destroy_recursive(node->left);
        destroy_recursive(node->right);
        delete node;
    }
}

Rb_tree::~Rb_tree() {
    destroy_recursive(this->root);
    delete this->NIL;
    int i = 0;
}

NodePtr Rb_tree::search_tree(NodePtr node, unsigned long int key) {
    if (node == this->NIL || key == node->key) {
        return node;
    }
    else if (key < node->key) {
        return search_tree(node->left, key);
    }
    else {
        return search_tree(node->right, key);
    }
}


NodePtr Rb_tree::search(unsigned long int k) {
    return search_tree(this->root, k);
}

bool Rb_tree::is_present(unsigned long key) {
    if (search(key) != this->NIL) {
        return true;
    }
    else {
        return false;
    }
}
// rotate left at node x
void Rb_tree::leftRotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != this->NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// rotate right at node x
void Rb_tree::rightRotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != this->NIL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// fix the red-black tree
void Rb_tree::rebalance_insert(NodePtr node) {
    NodePtr u;
    while (node->parent->color) {
        if (node->parent == node->parent->parent->right) {
            u = node->parent->parent->left; // uncle
            if (u->color == 1) {
                // case 3.1
                u->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    // case 3.2.1
                    node->parent->color = 0;
                    node->parent->parent->color = 1;
                    leftRotate(node->parent->parent);
                }
                else {
                    // case 3.2.2
                    node = node->parent;
                    rightRotate(node);
                }
            }
        }
        else {
            u = node->parent->parent->right; // uncle

            if (u->color == 1) {
                // mirror case 3.1
                u->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    // mirror case 3.2.1
                    node->parent->color =0;
                    node->parent->parent->color = 1;
                    rightRotate(node->parent->parent);
                }
                else {
                    node = node->parent;
                    leftRotate(node);
                }
            }
        }
        if (node == root) {
            break;
        }
    }
    root->color = 0;
}

NodePtr Rb_tree::build_node(unsigned long key) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->key = key;
    node->left = this->NIL;
    node->right = this->NIL;
    node->color = 1; // New node must be red
    return node;
}


bool Rb_tree::insert(unsigned long int key) {
    if (search(key) != this->NIL){
        return false;
    }

    NodePtr node = build_node(key);
    NodePtr node_parent = nullptr;
    NodePtr x = this->root;
    this->tree_size ++;
    while (x != this->NIL) {
        node_parent = x; // Last node before this->NIL
        if (node->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    if (node_parent == nullptr) { // Empty Tree
        this->root = node;
    } else if (node->key < node_parent->key) {
        node->parent = node_parent;
        node_parent->left = node;
    } else {
        node->parent = node_parent;
        node_parent->right = node;
    }
    // if new node is a root node, simply return
    if (node->parent == nullptr){
        node->color = 0; //Root must be black
        return true;
    }
    // if the grandparent is null, simply return
    else if (node->parent->parent == nullptr) {
        return true;
    }
    else {
        rebalance_insert(node);
        return true;
    }

}

// find the node with the minimum key
NodePtr Rb_tree::minimum(NodePtr node) {
    while (node->left != this->NIL) {
        node = node->left;
    }
    return node;
}

// find the node with the maximum key
NodePtr Rb_tree::maximum(NodePtr node) {
    while (node->right != this->NIL) {
        node = node->right;
    }
    return node;
}

void Rb_tree::transplant(NodePtr u, NodePtr v){
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void Rb_tree::delete_node(unsigned long int key) {
           // find the node containing key
           NodePtr node = search(key);
           NodePtr x, y;

           if (node == this->NIL) {
               return;
           }

           y = node; //Copy
           bool y_original_color = y->color;
           if (node->left == this->NIL) {
               x = node->right;
               transplant(node, node->right);
           } else if (node->right == this->NIL) {
               x = node->left;
               transplant(node, node->left);
           } else {
               y = minimum(node->right);
               y_original_color = y->color;
               x = y->right;
               if (y->parent == node) {
                   x->parent = y;
               } else {
                   transplant(y, y->right);
                   y->right = node->right;
                   y->right->parent = y;
               }

               transplant(node, y);
               y->left = node->left;
               y->left->parent = y;
               y->color = node->color;
           }
           if (y_original_color == 0){
                rebalance_delete(x);
           }
           // Memory release deleted node
           delete node;
           this->tree_size--;
}

void Rb_tree::rebalance_delete(NodePtr node) {
    NodePtr s;
    while (node != root && node->color == 0) {
        if (node == node->parent->left) {
            s = node->parent->right;
            if (s->color == 1) {
                // case 3.1
                s->color = 0;
                node->parent->color = 1;
                leftRotate(node->parent);
                s = node->parent->right;
            }

            if (s->left->color == 0 && s->right->color == 0) {
                // case 3.2
                s->color = 1;
                node = node->parent;
            } else {
                if (s->right->color == 0) {
                    // case 3.3
                    s->left->color = 0;
                    s->color = 1;
                    rightRotate(s);
                    s = node->parent->right;
                }

                // case 3.4
                s->color = node->parent->color;
                node->parent->color = 0;
                s->right->color = 0;
                leftRotate(node->parent);
                node = root;
            }
        } else {
            s = node->parent->left;
            if (s->color == 1) {
                // Mirror case 3.1
                s->color = 0;
                node->parent->color = 1;
                rightRotate(node->parent);
                s = node->parent->left;
            }

            if (s->right->color == 0 && s->right->color == 0) {
                // Mirror case 3.2
                s->color = 1;
                node = node->parent;
            } else {
                if (s->left->color == 0) {
                    // Mirror case 3.3
                    s->right->color = 0;
                    s->color = 1;
                    leftRotate(s);
                    s = node->parent->left;
                }

                // Mirror case 3.4
                s->color = node->parent->color;
                node->parent->color = 0;
                s->left->color = 0;
                rightRotate(node->parent);
                node = root;
            }
        }
    }
    node->color = 0;
}

unsigned long int Rb_tree::size() {
    return this->tree_size;
}

bool Rb_tree::empty() {
    if (this->tree_size > 0) {
        return false;
    } else {
        return true;
    }
}

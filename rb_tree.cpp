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

void Rb_tree::destroy_recursive(NodePtr node) {
    if (node == this->NIL){
        return;
    }
    if (node->left == this->NIL && node->right == this->NIL) {
        node = nullptr;
        delete node;
        return;
    }
    if (node->left != this->NIL) {
        destroy_recursive(node->left);
        node = nullptr;
        delete node;
        return;
    }
    if (node->right != this->NIL){
        destroy_recursive(node->right);
        node = nullptr;
        delete node;
        return;
    }
}

Rb_tree::~Rb_tree() {
    destroy_recursive(this->root);
    if (this->tree_size) {
        this->tree_size = 0;
    }
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
//    if (this->tree_size == 0) {
//        return false;
//    }
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

NodePtr Rb_tree::build_node(unsigned long key) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->key = key;
    node->left = this->NIL;
    node->right = this->NIL;
    node->color = 1; // New node must be red
    return node;
}

// fix the red-black tree
void Rb_tree::rebalance_insert(NodePtr node) {
    NodePtr u, p;
    if (node == this->root) {
        this->root->color = 0;
        return;
    }
    p = node->parent;
    if (p->color) {
        if (node->parent == node->parent->parent->right) {
            u = node->parent->parent->left; // uncle
            // u color is Black
            if (u->color == 0) {
                if (node == p->left) {
                    rightRotate(p);
                    leftRotate(node->parent);
                    node->color = 0;
                    node->left->color = 1;
                } else {
                    leftRotate(p->parent);
                    p->color = 0;
                    p->left->color = 1;
                }
            // u color is Red
            } else {
                u->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
                rebalance_insert(node);
                return;
            }
        } else {
            u = node->parent->parent->right;
            if (u->color == 0) {
                if (node == p->right) {
                    leftRotate(p);
                    rightRotate(node->parent);
                    node->color = 0;
                    node->right->color = 1;
                } else {
                    rightRotate(p->parent);
                    p->color = 0;
                    p->right->color = 1;
                }

            } else {
                u->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
                rebalance_insert(node);
                return;
            }
        }
    } else {
        return;
    }
}

bool Rb_tree::insert(unsigned long int key) {
    NodePtr x = this->root;
    NodePtr node_parent = nullptr;
    while (x != this->NIL) {
        node_parent = x; // Last node before this->NIL
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            return false;
        }
    }
    NodePtr node = build_node(key);
    this->tree_size++;
    if (node_parent == nullptr) { // Empty Tree
        this->root = node;
        node->color = 0;
        return true;
    } else if (node->key < node_parent->key) {
        node->parent = node_parent;
        node_parent->left = node;
    } else {
        node->parent = node_parent;
        node_parent->right = node;
    }
    // if the grandparent is null, simply return
    if (node->parent->parent == nullptr) {
        return true;
    } else {
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
        this->root = v;
    } else if (u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

NodePtr Rb_tree::replacement(NodePtr node) {
    if (node->left != this->NIL && node->right != this->NIL) {
        return minimum(node->right);
    }
    if (node->right == this->NIL) {
        return  node->left;
    }
    else {
        return  node->right;
    }
}

void Rb_tree::delete_key(unsigned long int key) {
    NodePtr v = search(key);
    if (v == this->NIL) {
        return;
    }
    this->tree_size--;
    delete_node(v);
}

void Rb_tree::delete_node(NodePtr v) {

    NodePtr u = replacement(v);
    NodePtr p = v->parent;
    // If v is a leaf
    if (u == this->NIL) {      
        if (v == this->root) {
            this->root = this->NIL;
        } else {
                //v is red
                if (v->color == 1) {
                    if (v == p->left) {
                        p->left = this->NIL;
                    } else {
                        p->right = this->NIL;
                    }
                }
                //v is black
                else {
                        rebalance_delete(v); // Double Black on v
                        p =  v->parent;
                        if (v == p->left) {
                            p->left = this->NIL;
                        } else {
                            p->right = this->NIL;
                        }
                        NodePtr p1 = v->parent;
                        if (p != p1) {
                            p = p1;
                        }
                }
        }
        delete v;        
        return;       
    // v is not a leaf
    } else {
        v->key = u->key;
        delete_node(u);
        return;
    }
}

void Rb_tree::rebalance_delete(NodePtr node) {
    if (node == this->root) {
        return;
    }
    NodePtr s, p;
    p = node->parent;
    if (node == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }
    if (s == this->NIL) { //CHECK CORECTENESS
        if (p->color == 1) {
            p->color = 0;
        } else {
            //Pass DB to parent
            rebalance_delete(p);
        }
        return;
    } else {
        if (s->color == 0) {
            if (s->left->color == 0 && s->right->color == 0) {
                s->color = 1;
                if (p->color) {
                    p->color = 0;
                    return;
                } else {
                    rebalance_delete(p);
                    return;
                }
            }
            else if (s == p->right) {
                if (s->left->color == 1) {
                    s->color = 1;
                    s->left->color = 0;
                    rightRotate(s);
                    rebalance_delete(node);
                    return;
                } else {
                    bool temp = p->color;
                    p->color = s->color;
                    s->color = temp;
                    leftRotate(p);
                    s->right->color = 0;
                }
            } else {
                  if (s->right->color == 1) {
                      s->color = 1;
                      s->right->color = 0;
                      leftRotate(s);
                      rebalance_delete(node);
                      return;
                  } else {
                      bool temp = p->color;
                      p->color = s->color;
                      s->color = temp;
                      rightRotate(p);
                      s->left->color = 0;
                      return;
                  }
            }
        }
        else {
            s->color = 0;
            p->color = 1;
            if (s == p->left) {
                rightRotate(p);
            } else {
                leftRotate(p);
            }
            rebalance_delete(node);
        }
    }
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

void Rb_tree::iterate() {
    this->queue.push(this->root);
}

NodePtr Rb_tree::next() {
    NodePtr n;
    if (!this->queue.empty()) {
        n = this->queue.front();
        this->queue.pop();
        if (n->left != this->NIL) {
            this->queue.push(n->left);
        }
        if (n->right != this->NIL) {
            this->queue.push(n->right);
        }
        return n;
    } else {
        return this->NIL;
    }
}

std::vector<unsigned long int> Rb_tree::bfs(){
    std::vector<unsigned long int> node_keys;
    NodePtr n;
    if (this->tree_size == 0) {
        return node_keys;
    }
    iterate();
    n = next();
    while (n != this->NIL) {
        node_keys.push_back(n->key);
        n = next();
    }
    return node_keys;
}

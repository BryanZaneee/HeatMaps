#include <iostream>

using namespace std;

struct Node {
    int key; // holds the key
    tuple<double, double, int> values;
    Node *parent; // pointer to the parent
    Node *left; // pointer to left child
    Node *right; // pointer to right child
    int color; // 1 -> Red, 0 -> Black
};

typedef Node *NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
    NodePtr root;
    NodePtr TNULL;

    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->key = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }

    void inOrderHelper(NodePtr node, vector<double>& latitudeVec, vector<double>& longitudeVec, vector<double>& totalVehiclesVec) {
        if (node != TNULL) {
            inOrderHelper(node->left, latitudeVec, longitudeVec, totalVehiclesVec);

            auto values = node->values;
            double latitude = get<0>(values);
            double longitude = get<1>(values);
            double totalVehicles = get<2>(values);

            latitudeVec.push_back(latitude);
            longitudeVec.push_back(longitude);
            totalVehiclesVec.push_back(totalVehicles);

            inOrderHelper(node->right, latitudeVec, longitudeVec, totalVehiclesVec);
        }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
        if (node == TNULL || key == node->key) {
            return node;
        }

        if (key < node->key) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // fix the red-black tree
    void fixInsert(NodePtr k){
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // uncle
                if (u->color == 1) {
                    // case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        // case 3.2.2
                        k = k->parent;
                        rightRotate(k);
                    }
                    // case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right; // uncle

                if (u->color == 1) {
                    // mirror case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        // mirror case 3.2.2
                        k = k->parent;
                        leftRotate(k);
                    }
                    // mirror case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

public:
    //constructor
    RBTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    //inorder traversal
    void inorder(vector<double>& latitudeVec, vector<double>& longitudeVec, vector<double>& totalVehiclesVec) {
        inOrderHelper(this->root, latitudeVec, longitudeVec, totalVehiclesVec);
    }

    // search the tree for the key k
    // and return the corresponding node
    NodePtr searchTree(int k) {
        return searchTreeHelper(this->root, k);
    }

    // rotate left at node x
    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
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
    void rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
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

    // insert the key to the tree in its appropriate position
    // and fix the tree
    void insert(int key, double latitude, double longtitude, int value3) {
        // Ordinary Binary Search Insertion
        NodePtr node = new Node;
        node->parent = nullptr;
        node->key = key;
        node->values = make_tuple(latitude, longtitude, value3);
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1; // new node must be red

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        // y is parent of x
        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }

        // if new node is a root node, simply return
        if (node->parent == nullptr){
            node->color = 0;
            return;
        }

        // if the grandparent is null, simply return
        if (node->parent->parent == nullptr) {
            return;
        }

        // Fix the tree
        fixInsert(node);
    }
};

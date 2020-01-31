#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <exception>
#include <initializer_list>
#include <functional>

template<class T>
class AvlTree
{

private:

    class AvlNode {

    public:

        T element;
        AvlNode* left;
        AvlNode* right;
        int height;

        AvlNode(): T(), left(nullptr), right(nullptr), height(0) {}
        AvlNode(const T& e, AvlNode* l, AvlNode* r, int h = 0): element(e), left(l), right(r), height(h) {}

    };

    AvlNode* root;

    int height(AvlNode*) const;
    int max(int, int) const;

    AvlNode* findMin(AvlNode*) const;
    AvlNode* findMax(AvlNode*) const;
    bool contains(const T&, AvlNode*) const;

    AvlNode* find(const T&, AvlNode*);

    void insert(const T&, AvlNode*&);
    void makeEmpty(AvlNode*&);
    AvlNode* deepCopy(AvlNode*) const;

    void rotateWithLeftChild(AvlNode*&);
    void rotateWithRightChild(AvlNode*&);
    void doubleWithLeftChild(AvlNode*&);
    void doubleWithRightChild(AvlNode*&);

    void print(AvlNode*);
    void toFile(AvlNode*, std::ofstream&);

    unsigned int countNodes(AvlNode*);

    void visitNode(AvlNode*, std::function<void(T)> func);

public:

    AvlTree(): root() {}
    AvlTree(const AvlTree&);
    AvlTree(std::initializer_list<T>);
    ~AvlTree();

    AvlTree& operator=(const AvlTree&);

    const T& findMin() const;
    const T& findMax() const;
    bool contains(const T& x) const;
    bool isEmpty() const;

    T* find(const T&);

    void makeEmpty();
    void insert(const T& x);

    void print();
    void toFile(std::ofstream&);

    unsigned int countNodes();

    void visitNode(std::function<void(T)> func);

};

template<class T>
AvlTree<T>::AvlTree(const AvlTree& rhs) {

    //Initialize root and use assignment operator

    root = nullptr;
    *this = rhs;

}

template<class T>
AvlTree<T>::AvlTree(std::initializer_list<T> il) {

    //Constructor of form:
    // AvlTree<T> tree = {elements...};

    root = nullptr;

    for (auto& i : il) {

        insert(i);

    }

}

template<class T>
AvlTree<T>::~AvlTree() {

    makeEmpty();

}

template<class T>
AvlTree<T>& AvlTree<T>::operator=(const AvlTree& rhs) {

    //Assignment: empty the current tree and replace with copy of rhs

    if (this != &rhs) {

        makeEmpty();
        root = deepCopy(rhs.root);

    }

    return *this;

}

template<class T>
int AvlTree<T>::height(AvlTree<T>::AvlNode* t) const {

    return t == nullptr ? -1 : t->height;

}

template<class T>
int AvlTree<T>::max(int lhs, int rhs) const {

    return lhs > rhs ? lhs : rhs;

}

template<class T>
void AvlTree<T>::rotateWithLeftChild(AvlNode*& k2) {

    //Case 1 rotation

    AvlNode* lt = k2->left; //nullptr
    k2->left = lt->right; //lt becomes new root of the tree
    lt->right = k2; //k2 is now right child of new root

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    lt->height = max(height(lt->left), k2->height) + 1; //adjust to height of larger subtree for each node

    k2 = lt; //change ptr to new root

}

template<class T>
void AvlTree<T>::rotateWithRightChild(AvlNode*& k1) {

    //Case 4 rotation, mirror of case 1 rotation

    AvlNode* rt = k1->right; //nullptr
    k1->right = rt->left; //rt becomes new root of the tree
    rt->left = k1; //k2 is now left child of rt

    k1->height = max(height(k1->left), height(k1->right)) + 1;
    rt->height = max(height(rt->right), k1->height) + 1; //adjust heights

    k1 = rt; //change ptr for new root

}

template<class T>
void AvlTree<T>::doubleWithLeftChild(AvlNode*& k3) {

    //Case 2 rotation

    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);

}

template<class T>
void AvlTree<T>::doubleWithRightChild(AvlNode*& k1) {

    //Case 3 rotation

    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);

}

template<class T>
void AvlTree<T>::insert(const T& x, AvlNode*& t) {

    if (t == nullptr)
        t = new AvlNode(x, nullptr, nullptr);
    else if (x < t->element) {

        insert(x, t->left);
        if (height(t->left) - height(t->right) == 2) {

            if (x < t->left->element)
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);

        }

    }
    else if (t->element < x) {

        insert(x, t->right);
        if (height(t->right) - height(t->left) == 2) {

            if (t->right->element < x)
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);

        }

    }
    else; //Duplicates are not inserted

    t->height = max(height(t->left), height(t->right)) + 1;

}

template<class T>
void AvlTree<T>::insert(const T& x) {

    insert(x, root);

}

template<class T>
typename AvlTree<T>::AvlNode* AvlTree<T>::findMin(AvlNode* t) const {

    //Returns the smallest value of given subtree

    if (t == nullptr)
        return nullptr;
    else if (t->left == nullptr)
        return t;
    else
        return findMin(t->left);
}

template<class T>
typename AvlTree<T>::AvlNode* AvlTree<T>::findMax(AvlNode* t) const {

    //Returns largest value of given subtree

    if (t == nullptr)
        return nullptr;
    else if (t->right == nullptr)
        return t;
    else
        return findMin(t->right);


}

template<class T>
const T& AvlTree<T>::findMin() const {

    //Find the smallest value in the tree

    if (isEmpty())
        throw std::underflow_error("Underflow exception");
    return findMin(root)->element;

}

template<class T>
const T& AvlTree<T>::findMax() const {

    //Find the smallest value in the tree

    if (isEmpty())
        throw std::underflow_error("Underflow exception");
    return findMax(root)->element;

}

template<class T>
bool AvlTree<T>::contains(const T& x, AvlNode* t) const {

    if (t == nullptr)
        return false; //value not found

    else if (x < t->element)
        return contains(x, t->left); //search left subtree

    else if (t->element < x)
        return contains(x, t->right); //search right subtree

    else
        return true;

}

template<class T>
bool AvlTree<T>::contains(const T& x) const {

    return contains(x, root);

}

template<class T>
typename AvlTree<T>::AvlNode* AvlTree<T>::find(const T& x, AvlNode* t) {

    if (t == nullptr)
        return nullptr; //value not found

    else if (x < t->element)
        return find(x, t->left); //search left subtree

    else if (t->element < x)
        return find(x, t->right); //search right subtree

    else
        return t;

}

template<class T>
T* AvlTree<T>::find(const T& x) {

    //Returns a ptr to x if it's in the tree, or nullptr if not

    AvlNode* returnVal = find(x, root);

    if (returnVal == nullptr)
        return nullptr;

    return &returnVal->element;

}

template<class T>
bool AvlTree<T>::isEmpty() const {

    return root == nullptr;

}

template<class T>
void AvlTree<T>::makeEmpty(AvlNode*& t) {

    //Empties a subtree

    if (t != nullptr) {

        makeEmpty(t->left);
        makeEmpty(t->right);

        delete t;
        t = nullptr;

    }

}

template<class T>
void AvlTree<T>::makeEmpty() {

    //Empties the tree

    makeEmpty(root);

}

template<class T>
typename AvlTree<T>::AvlNode* AvlTree<T>::deepCopy(AvlNode* t) const {

    //Deep copies the subtree at t

    if (t == nullptr)
        return nullptr;

    else
        return new AvlNode(t->element, deepCopy(t->left), deepCopy(t->right), height(t));

}

template<class T>
void AvlTree<T>::print(AvlNode* t) {

    //Print elements, for debugging
    if (t != nullptr) {

        print(t->left);
        std::cout << t->element << std::endl;
        print(t->right);

    }

}

template<class T>
void AvlTree<T>::print() {

    print(root);

}

template<class T>
void AvlTree<T>::toFile(AvlNode* t, std::ofstream& file) {

    if (t != nullptr) {

        toFile(t->left, file);
        file << t->element;
        toFile(t->right, file);

    }

}

template<class T>
void AvlTree<T>::toFile(std::ofstream& file) {

    //Recursively outputs all elements into given file

    toFile(root, file);

}

template<class T>
unsigned int AvlTree<T>::countNodes(AvlNode* t) {

    //Returns the num of nodes in the subtree t

    unsigned int count = 0;
    if (t == nullptr) return 0;
    if (height(t) == 0) return 1;

    count += countNodes(t->left);
    count += countNodes(t->right);

    return count + 1; //add 1 for current node

}

template<class T>
unsigned int AvlTree<T>::countNodes() {

    return countNodes(root);

}

template<class T>
void AvlTree<T>::visitNode(AvlNode* t, std::function<void(T)> func) {

    //Allows a void function to act on all values in the subtree t

    if (t == nullptr) return;
    else {

        visitNode(t->left, func);
        func(t->element);
        visitNode(t->right, func);

    }


}

template<class T>
void AvlTree<T>::visitNode(std::function<void(T)> func) {

    visitNode(root, func);

}



#endif // AVLTREE_H

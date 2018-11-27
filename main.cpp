#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <cmath>

template<typename type1, typename type2>
struct Comparator {
  bool operator()(std::pair<type1, type2> left, std::pair<type1, type2> right) {
    return left.second > right.second;
  }
};
//
// Node
//
template<typename type1, typename type2>
struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  std::pair<type1, type2> value;
  Node(type1 v1, type2 v2);
  Node(std::pair<type1, type2> v);
};

template<typename type1, typename type2>
Node<type1,type2>::Node(type1 v1, type2 v2) : value(std::pair<type1, type2>(v1, v2)) {}

template<typename type1, typename type2>
Node<type1, type2>::Node(std::pair<type1, type2> v) : value(v) {}
//
// Treap
//
template<typename type1, typename type2>
class Treap {
  public:
    Treap();
    Treap(Node<type1, type2>*& root);
    Treap(const std::list<std::pair<type1, type2>>& vec);
    ~Treap();

    size_t max_depth() const;
    void split(Node<type1, type2>*& left,
        Node<type1, type2>*& right,
        type1 key);
    void insert(type1 first, type2 second);
  private:
    void split_(Node<type1, type2>* current,
        Node<type1, type2>*& left,
        Node<type1, type2>*& right,
        type1 key);
    void delete_func_(Node<type1, type2>*& root);
    Node<type1, type2>* root_;
};

template<typename type1, typename type2>
Treap<type1, type2>::Treap() : root_(nullptr) {}

template<typename type1, typename type2>
Treap<type1, type2>::Treap(Node<type1, type2>*& root) : root_(root) {}

template<typename type1, typename type2>
Treap<type1, type2>::Treap(const std::list<std::pair<type1, type2>>& vec) {
  root_ = new Node<type1, type2>(*(vec.begin()));
  Node<type1, type2>* root = root_;
  for(auto iterator = next(vec.begin()); iterator != vec.end(); ++iterator) {
    while(true) {
      if(iterator->first < root->value.first) {
        if(root->left == nullptr) {
          root->left = new Node<type1, type2>(*iterator);
          root = root_;
          break;
        }
        root = root->left;
      }
      else {
        if(root->right == nullptr) {
          root->right = new Node<type1, type2>(*iterator);
          root = root_;
          break;
        }
        root = root->right;
      }
    }
  }
}

template<typename type1, typename type2>
Treap<type1, type2>::~Treap() { 
  delete_func_(root_);
}

template<typename type1, typename type2>
void Treap<type1, type2>::delete_func_(Node<type1, type2>*& root) {
  if(!root) 
    return;
  delete_func_(root->left);
  delete_func_(root->right);
  delete root;
}

template<typename type1, typename type2>
void Treap<type1, type2>::split_(Node<type1, type2>* current,
    Node<type1, type2>*& left,
    Node<type1, type2>*& right,
    type1 key) {
  if(current == nullptr) {
    left = nullptr;
    right = nullptr;
  }
  else if(current->value.first <= key) {
    split_(current->right, current->right, right, key);
    left = current;
  }
  else {
    split_(current->left, left, current->left, key);
    right = current;
  }
}
template <typename type1, typename type2>
Node<type1,type2>* merge(Node<type1, type2>* left,
    Node<type1, type2>* right) {
  if(!left) {
    return right;
  }
  if(!right) {
    return left;
  }
  if(left->value.second < right->value.second) {
    left->right = merge(left->right, right); 
    return left;
  }
  else {
    right->left = merge(left, right->left);
    return right;
  }
}

template<typename type1, typename type2>
void Treap<type1,type2>::split(Node<type1, type2>*& left,
    Node<type1, type2>*& right,
    type1 key) {
  split_(root_, left, right, key);
  root_ = nullptr;
}

template<typename type1, typename type2>
void Treap<type1,type2>::insert(type1 first, type2 second) { 
  Node<type1,type2>* current = root_;
  Node<type1,type2>* parent = nullptr;
  Node<type1,type2>* left = nullptr;
  Node<type1,type2>* right = nullptr;
  bool left_child = false;
  if(!root_){
    root_ = new Node<type1, type2>(first, second);
    return;
  }
  if(root_->value.second >= second) {
    while(current && current->value.second >= second) {
      if(current->value.first < first) {
        parent = current;
        current = current->right;
        left_child = false;
      }
      else {
        parent = current;
        current = current->left;
        left_child = true;
      }
    }
    this->split_(current ,left, right, first);
    if(left_child) {
      parent->left = new Node<type1,type2>(first, second);
      parent->left->left = left;
      parent->left->right = right;
    }
    else {
      parent->right = new Node<type1,type2>(first, second);
      parent->right->left = left;
      parent->right->right = right; 
    }
  }
  else {
    this->split(left, right, first);
    Node<type1, type2>* node = new Node<type1, type2>(first, second);
    node->left = left;
    node->right = right;
    root_ = node;
  }
}
//
// Binary Tree
//
template<typename type1, typename type2>
class Tree {
  private:
    Node<type1, type2> * root_;
    void delete_func_(Node<type1, type2>*& node);
  public:
    Tree();
    ~Tree();
    void add(type1, type2);
    size_t max_depth() const;
};
 
template<typename type1, typename type2>
void Tree<type1,type2>::delete_func_(Node<type1,type2>*& node) {
  if(node) {
    delete_func_(node->left);
    delete_func_(node->right);
    delete node;
  }
}

template<typename type1, typename type2>
Tree<type1,type2>::Tree() : root_(nullptr) {}

template<typename type1, typename type2>
Tree<type1,type2>::~Tree()  { delete_func_(root_);}

template<typename type1, typename type2>
void Tree<type1,type2>::add(type1 first, type2 second) {
  Node<type1,type2>* iterator = root_;
  if(root_ == nullptr) {
    root_ = new Node<type1,type2>(first,second);
    return;
  }
  while(true) {
    if(first < iterator->value.first) {
      if(iterator->left == nullptr) {
        iterator->left = new Node<type1,type2>(first, second);
        return;
      }
      iterator = iterator->left;
    }
    else {
      if(iterator->right == nullptr) {
        iterator->right = new Node<type1,type2>(first, second);
        return;
      }
      iterator = iterator->right;
    }
  }
}
//
// Max Depth
//
template<typename type1, typename type2>
size_t Treap<type1,type2>::max_depth() const {
  if(!root_)
    return 0;
  size_t height = 0;
  Node<type1,type2>* current = root_;
  std::list<Node<type1,type2>*> stack1;
  std::list<Node<type1,type2>*> stack2;
  stack1.push_back(root_);
  while(!stack1.empty()) {
    current = stack1.back();
    if(!stack2.empty() && current == stack2.back()) {
      if(stack2.size() > height) {
        height = stack2.size();
      }
      stack1.pop_back();
      stack2.pop_back();
    }
    else {
      stack2.push_back(current);
      if(current->right) {
        stack1.push_back(current->right);
      }
      if(current->left) {
        stack1.push_back(current->left);
      }
    }
  }
  return height;
}

template<typename type1, typename type2>
size_t Tree<type1,type2>::max_depth() const {
  if(!root_)
    return 0;
  size_t height = 0;
  Node<type1,type2>* current = root_;
  std::list<Node<type1,type2>*> stack1;
  std::list<Node<type1,type2>*> stack2;
  stack1.push_back(root_);
  while(!stack1.empty()) {
    current = stack1.back();
    if(!stack2.empty() && current == stack2.back()) {
      if(stack2.size() > height) {
        height = stack2.size();
      }
      stack1.pop_back();
      stack2.pop_back();
    }
    else {
      stack2.push_back(current);
      if(current->right) {
        stack1.push_back(current->right);
      }
      if(current->left) {
        stack1.push_back(current->left);
      }
    }
  }
  return height;
}


int main(int argc, char** argv) {
  Treap<int,int> treap;
  Tree<int,int> bin_tree;
  int first = 0;
  int second = 0;
  int n = 0;
  std::cin >> n;
  for(int i = 0; i < n; ++i) {
    std::cin >> first >> second;  
    treap.insert(first, second);
    bin_tree.add(first, second);
  }
  int h1 = (int)treap.max_depth();
  int h2 = (int)bin_tree.max_depth();
  std::cout << abs(h1 - h2) << std::endl;
  return 0;
}

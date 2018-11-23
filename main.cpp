#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

template<typename type1, typename type2>
struct Comparator {
	bool operator()(std::pair<type1, type2> left, std::pair<type1, type2> right) {
		return left.second > right.second;
	}
};

template<typename type1, typename type2>
struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
	std::pair<type1, type2> value;
	Node(std::pair<type1, type2> v);
};

template<typename type1, typename type2>
Node<type1, type2>::Node(std::pair<type1, type2> v)
	: value(v) {}

	template<typename type1, typename type2>
	class Treap {
		public:
			Treap();
			Treap(const std::vector<std::pair<type1, type2>>& vec);
			~Treap();
			Node<type1, type2>* root_;
		private:
			void delete_func(Node<type1, type2>* root);
	};

template<typename type1, typename type2>
Treap<type1, type2>::Treap() : root_(nullptr) {}

template<typename type1, typename type2>
Treap<type1, type2>::Treap(const std::vector<std::pair<type1, type2>>& vec) {
	root_ = new Node<type1, type2>(*(vec.begin()));
	Node<type1, type2>* root = root_;
	for(auto iterator = vec.begin() + 1; iterator != vec.end(); ++iterator) {
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
	delete_func(root_);
}

template<typename type1, typename type2>
void in_order_print(const Node<type1, type2>* root) {
	if(root) {
		in_order_print(root->left);
		std::cout << root->value.first << ' ' << root->value.second << std::endl;
		in_order_print(root->right);
	}
}
		
template<typename type1, typename type2>
void Treap<type1, type2>::delete_func(Node<type1, type2>* root) {
	if(root) {
		delete_func(root->left);
		delete_func(root->right);
		delete root;
	}
}

int main(int argc, char** argv) {
	int n = 0;
	int first = 0;
	int second = 0;
	std::cin >> n;
	std::vector<std::pair<int,int>> vec(n);
	for(int i = 0; i < n; ++i) {
		std::cin >> first >> second;
		vec[i] = std::move(std::pair<int, int>(first, second));
	}
	std::sort(vec.begin(), vec.end(), Comparator<int,int>());
	for(const auto& x: vec) {
		std::cout << x.first << ' ' << x.second << std::endl;
	}
	std::cout << std::endl;
	Treap<int, int> treap(vec);
	in_order_print(treap.root_);

	return 0;
}

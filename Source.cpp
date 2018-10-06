#include <iostream>

using namespace std;
const int INF = (int) 1e8;

template<typename T>
class Node {
public:
	T value;
	Node<T> *left;
	Node<T> *right;
	Node<T> *parent;

	Node(const Node<T> &last) : value(last.value) {
		if (last.left != nullptr) {
			left = new Node<T>(*last.left);
			left->parent = this;
		}
		if (last.right != nullptr) {
			right = new Node<T>(*last.right);
			right->parent = this;
		}
	}

	Node(const T &item, Node<T> *lptr = nullptr,
		Node<T> *rptr = nullptr, Node<T> *pptr = nullptr) :
		value(item), left(lptr), right(rptr), parent(pptr) {}

	~Node() {
		if (left != nullptr) {
			delete left;
		}
		if (right != nullptr) {
			delete right;
		}
	}
};

template<typename T>
class constBstIterator {
public:
	constBstIterator(Node<T> *p) : nodePtr(p) {}

	bool operator==(const constBstIterator &item) const {
		return item.nodePtr == this->nodePtr;
	}

	bool operator!=(const constBstIterator &item) const {
		return item.nodePtr != this->nodePtr;
	}

	T &operator*() const {
		return this->nodePtr->value;
	}

	constBstIterator operator++(int) {
		if (nodePtr->value == INF) {
			while (nodePtr->left != nullptr) {
				nodePtr = nodePtr->left;
			}
		}
		else if (nodePtr->right != nullptr) {
			nodePtr = nodePtr->right;
			while (nodePtr->left != nullptr) {
				nodePtr = nodePtr->left;
			}
		}
		else {
			Node<T> *p = nodePtr->parent;
			while (p != nullptr && nodePtr == p->right) {
				nodePtr = p;
				p = p->parent;
			}
			nodePtr = p;
		}

		return *this;
	};

	constBstIterator operator--(int) {
		if (nodePtr->left != nullptr) {
			nodePtr = nodePtr->left;

			while (nodePtr->right != nullptr) {
				nodePtr = nodePtr->right;
			}
		}
		else {
			Node<T> *p = p = nodePtr->parent;
			while (p != nullptr && nodePtr == p->left) {
				nodePtr = p;
				p = p->parent;
			}
			nodePtr = p;
		}
		return *this;
	};

private:
	Node<T> *nodePtr;
};

template<typename T, class Compare = std::less<T>>
class BST {
	typedef constBstIterator<T> iterator;
	typedef constBstIterator<T> const_iterator;
public:
	BST() {
		root = new Node<T>((int)INF);
	}

	BST(const BST &bst) {
		root = new Node<T>(INF);
		root->left = new Node<T>(*bst.root->left);
		root->left->parent = root;
	}

	BST(Node<T> *r) {
		root = r;
	}

	~BST() {
		delete root;
	}

	BST &operator=(const BST &bst) {
		_size = bst._size;
		root = new Node<T>((int)INF);
		root->left = new Node<T>(*bst.root->left);
		root->left->parent = root;
		return *this;
	}

	template<class InputIt>
	void assign(InputIt first, InputIt last) {
		for (InputIt i = first; i != last; i++) {
			insert(*(i));
		}
	}

	iterator begin() {
		Node<T> *n = root;
		while (n->left != nullptr) {
			n = n->left;
		}
		return iterator(n);
	}

	const_iterator cbegin() {
		Node<T> *n = root;
		while (n->left != nullptr) {
			n = n->left;
		}
		return iterator(n);
	};

	iterator end() {
		Node<T> *n = root;
		while (n->right != nullptr) {
			n = n->right;
		}
		return iterator(n);
	};

	const_iterator cend() {
		Node<T> *n = root;
		while (n->right != nullptr) {
			n = n->right;
		}
		return iterator(n);
	};

	iterator insert(const T &value) {
		_size++;
		Node<T> *n = root;
		while (n->left != nullptr || n->right != nullptr) {
			if (value < n->value) {
				n = n->left;
			}
			else if (value > n->value) {
				n = n->right;
			}
		}
		if (value > n->value) {
			n->right = new Node<T>(value, nullptr, nullptr, n);
			return iterator(n->right);
		}
		else {
			n->left = new Node<T>(value, nullptr, nullptr, n);
			return iterator(n->left);
		}
	};

	void remove(const T &value) {
		removeNode(NULL, false, root, value);
	}

	iterator find(const T &value) {
		Node<T> *n = root;
		while (n->left != nullptr || n->right != nullptr) {
			if (value < n->value) {
				n = n->left;
			}
			else if (value > n->value) {
				n = n->right;
			}
			else {
				break;
			}
		}
		if (value == n->value) {
			return iterator(n);
		}
		else {
			return iterator(root);
		}
	};

	const_iterator cfind(const T &value) {
		Node<T> *n = root;
		while (n->left != nullptr || n->right != nullptr) {
			if (value < n->value) {
				n = n->left;
			}
			else if (value > n->value) {
				n = n->right;
			}
			else break;
		}
		if (value == n->value) {
			return iterator(n);
		}
		else {
			return iterator(root);
		}
	};

	size_t size() const {
		return _size;
	};

	bool empty() const {
		return root->left == nullptr;
	};
private:
	Node<T> *root;
	int _size = 0;

	void removeNode(Node<T> *p, bool direction, Node<T> *node, const T &value) {
		if (value == node->value) {
			if ((node->left == nullptr) || (node->right == nullptr)) {
				Node<T> *tmp = node->left ? node->left : node->right;
				if (tmp == nullptr) {
					tmp = node;
					if (direction) {
						p->right = nullptr;
					}
					else {
						p->left = nullptr;
					}
					node = nullptr;
				}
				else {
					*node = *tmp;
				}
				delete tmp;
			}
			else {
				Node<T> *tmp = next(node);

				node->value = tmp->value;

				return removeNode(node, true, node->right, tmp->value);
			}
		}
		if (node != nullptr && value < node->value) {
			removeNode(node, false, node->left, value);
		}
		else if (node != nullptr) {
			removeNode(node, true, node->right, value);
		}
	};
};

int main() {
	BST<int> Tree;
	Tree.insert(8);
	Tree.insert(5);
	cout << "Fist element is : " << *Tree.begin() << '\n';
	cout << "Second element is : " << *Tree.begin()++ << '\n';
	system("pause");
	return 0;
}

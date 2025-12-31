#include <iostream>
#include <stack>
#include <functional>

/*
2_3. Обход дерева в порядке post-order

Дано число N≤10^4 и последовательность целых чисел из [-2^31...2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root,
если root→Key ≤ K, то узел K добавляется в правое поддерево root;
иначе в левое поддерево root. Выведите элементы в порядке post-order (снизу вверх).
*/

template <typename T, typename cmp = std::less<T>>
class BinaryTree {
	//struct Node;
public:

	BinaryTree() : root(nullptr) {

	}

	void insert(T val) {
		if (root == nullptr) {
			root = new Node(val);
			return;
		}

		Node* cur = root;
		Node* parent = nullptr;

		while (cur != nullptr) {
			parent = cur;

			if (comparator(val, cur->key)) {
				cur = cur->left;
			}
			else {
				cur = cur->right;
			}
		}

		if (comparator(val, parent->key)) {
			parent->left = new Node(val);
		}
		else {
			parent->right = new Node(val);
		}
	}

	void post_order() const {
		std::stack<Node*> st;

		Node* cur = root;
		Node* last = nullptr;

		while (!st.empty() || cur) {
			if (cur) {
				st.push(cur);
				cur = cur->left;
			}
			else {
				Node* top = st.top();

				if (top->right && top->right != last) {
					cur = top->right;
				}
				else {
					st.pop();
					std::cout << top->key << " ";
					last = top;
					cur = nullptr;
				}
			}
		}
	}

	~BinaryTree() {
		std::stack<Node*> st;

		st.push(root);

		while (!st.empty()) {
			Node* cur = st.top();
			st.pop();

			if (cur) {

				st.push(cur->left);
				st.push(cur->right);

				delete cur;
				cur = nullptr;
			}
		}
	}

	struct Node {
		Node(T val, Node* left = nullptr, Node* right = nullptr) : key(val), left(left), right(right) {
		}

		T key;
		Node* left{};
		Node* right{};
	};

private:

	Node* root{};

	cmp comparator;
};


int main() {

	BinaryTree<int64_t, std::less<int64_t>> tree;

	int N{};
	int64_t val{};

	std::cin >> N;

	for (int i = 0; i < N; ++i) {
		std::cin >> val;
		tree.insert(val);
	}

	tree.post_order();

	return 0;
}
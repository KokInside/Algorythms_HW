#include <iostream>
#include <vector>
#include <queue>

//////////////////////////////////////////

template <typename T, typename cmp = std::less<T>>
class B_tree {
public:
	class Node {

	public:

		Node() : is_leaf(false) {

		}

		std::vector<T> keys;
		std::vector<Node*> kids;
		bool is_leaf{};

	private:
		Node(size_t period) : is_leaf(true) {
			if (period < 2) {
				return;
			}
			keys.reserve(2 * period - 1);
			kids.reserve(2 * period);
		}

		Node(size_t period, T value) : Node(period) {

			keys.push_back(value);
			kids.push_back(nullptr);
			kids.push_back(nullptr);
		}

		friend Node* B_tree::createNode(size_t period);
		friend Node* B_tree::createNode(size_t period, T value);
	};

	B_tree(size_t period) : t(period) {
		if (period < 2) {
			root = nullptr;
			return;
		}

		root = createNode(period);
	}

	void insert(T value) {
		Node* cur = root;

		// full()
		if (root->keys.size() == 2 * t - 1) {
			Node* newRoot = createNode(t);

			root = newRoot;

			newRoot->is_leaf = false;

			newRoot->kids.push_back(cur);

			splitChild(newRoot, 0);

			cur = newRoot;
		}

		insertNotFull(cur, value);
	}

	void out(Node* node) const {
		for (int i = 0; i < node->keys.size(); ++i) {
			std::cout << node->keys[i] << " ";
		}
	}

	void bfs() const {
		std::queue<Node*> q;

		q.push(root);

		while (!q.empty()) {
			size_t rowSize = q.size();

			for (int i = 0; i < rowSize; ++i) {
				Node* cur = q.front();
				q.pop();
				out(cur);
				for (int j = 0; j < cur->kids.size(); ++j) {
					q.push(cur->kids[j]);
				}
			}

			std::cout << std::endl;
		}

	}

	~B_tree() {
		std::queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* cur = q.front();
			q.pop();

			if (cur) {
				for (int i = 0; i < cur->kids.size(); ++i) {
					q.push(cur->kids[i]);
				}
				delete cur;
				cur = nullptr;
			}
		}
	}

private:

	static Node* createNode(size_t period) {
		if (period < 2) {
			return nullptr;
		}

		return new Node(period);
	}

	static Node* createNode(size_t period, T value) {
		if (period < 2) {
			return nullptr;
		}

		return new Node(period, value);
	}

	// Вставка в неполный узел node 
	// исполняется до while (node->is_leaf == true)
	void insertNotFull(Node* node, T value) {

		Node* cur = node;

		while (cur->is_leaf == false) {
			size_t i = cur->keys.size() - 1;

			// а это делаем в while
			while (i >= 0 && comparator(value, cur->keys[i])) {
				--i;
			}
			++i;

			Node* child = cur->kids[i];

			// full()
			if (child->keys.size() == 2 * t - 1) {
				splitChild(cur, i);

				if (comparator(cur->keys[i], value)) {
					++i;
				}
			}

			//insertNotFull(node->kids[i], value);
			cur = cur->kids[i];
		}

		size_t i = cur->keys.size();


		// весь этот блок кидаем после while
		cur->keys.push_back(T());

		while (i >= 1 && comparator(value, cur->keys[i - 1])) {
			cur->keys[i] = cur->keys[i - 1];
			--i;
		}

		cur->keys[i] = value;

	}

	// для неполного внутреннего узла node разбиение его полного потомка childNumber
	// нерекурсивный
	void splitChild(Node* node, size_t childNumber) {
		Node* leftChild = node->kids[childNumber];

		size_t keysSize = leftChild->keys.size();
		size_t kidsSize = leftChild->kids.size();

		T mid = leftChild->keys[t - 1];

		Node* rightChild = createNode(t);

		rightChild->is_leaf = leftChild->is_leaf;

		rightChild->keys.insert(rightChild->keys.end(), leftChild->keys.begin() + t, leftChild->keys.end());

		leftChild->keys.resize(t - 1);

		if (leftChild->is_leaf == false) {
			rightChild->kids.insert(rightChild->kids.end(), leftChild->kids.begin() + t, leftChild->kids.end());
			leftChild->kids.resize(t);
		}

		node->keys.insert(node->keys.begin() + childNumber, mid);
		node->kids.insert(node->kids.begin() + childNumber + 1, rightChild);
	}

	const size_t t{};
	Node* root;
	cmp comparator;
};

//////////////////////////////////////////

int main() {

	int n{};
	std::cin >> n;

	B_tree<int> tree(n);

	int val{};

	while (std::cin >> val) {
		tree.insert(val);
	}

	tree.bfs();

	return 0;
}
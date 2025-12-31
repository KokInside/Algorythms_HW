#include <iostream>
#include <stack>
#include <queue>

///////////////////////////////////////////////

template<typename T, typename cmp = std::less<T>>
class avl_tree {
public:
	struct Node {
		Node(T val) : value(val), left(nullptr), right(nullptr), height(1), weight(1) {
		}

		T value{};
		Node* left{};
		Node* right{};
		size_t height{};
		size_t weight{};
	};

	avl_tree() {

	}

	void insert(T val) {
		if (root == nullptr) {
			root = new Node(val);
			++size_;
			return;
		}

		Node* cur = root;
		Node* par = nullptr;

		std::stack<Node*> insertStack;

		while (cur != nullptr) {

			insertStack.push(cur);

			if (cur->value == val) {
				while (!insertStack.empty()) {
					Node* prev = insertStack.top();
					prev = ballance(prev);
					insertStack.pop();
				}
				return;
			}

			par = cur;

			if (comparator(val, cur->value)) {
				par = cur;
				cur = cur->left;
			}
			else {
				par = cur;
				cur = cur->right;
			}
		}

		++size_;

		if (comparator(val, par->value)) {
			par->left = new Node(val);
		}
		else {
			par->right = new Node(val);
		}

		while (!insertStack.empty()) {
			Node* prev = insertStack.top();

			prev->left = ballance(prev->left);
			prev->right = ballance(prev->right);

			if (prev == root) {
				root = ballance(prev);
			}

			insertStack.pop();
		}
	}

	void BFS() {
		std::queue<Node*> q;
		if (root == nullptr) {
			return;
		}
		q.push(root);

		while (!q.empty()) {
			Node* cur = q.front();
			q.pop();

			if (cur) {
				std::cout << cur->value << " ";

				q.push(cur->left);
				q.push(cur->right);
			}
		}
	}

	size_t getWeight(Node* node) {
		return height(node);
	}

	Node* exists(T val) {

		if (root == nullptr) {
			return nullptr;
		}

		Node* cur = root;

		while (cur != nullptr) {

			if (cur->value == val) {
				return cur;
			}
			else if (comparator(val, cur->value)) {
				cur = cur->left;
			}
			else {
				cur = cur->right;
			}
		}

		return nullptr;

	}

	Node* next(T val) {

		Node* cur = root;
		Node* prev = nullptr;

		while (cur != nullptr) {
			if (comparator(val, cur->value)) {
				prev = cur;
				cur = cur->left;
			}
			else {
				cur = cur->right;
			}
		}

		return prev;

	}

	Node* prev(T val) {
		Node* cur = root;
		Node* prev = nullptr;

		while (cur != nullptr) {
			if (comparator(val, cur->value)) {
				cur = cur->left;
			}
			else {
				prev = cur;
				cur = cur->right;
			}
		}

		return prev;
	}

	void remove(T val) {
		if (root == nullptr) {
			return;
		}

		std::stack<Node*> st;

		Node* removedNode = exists_(val, st);

		if (removedNode == nullptr) {
			return;
		}

		--size_;

		if (removedNode->left == nullptr && removedNode->right == nullptr) {
			if (removedNode == root) {
				delete root;
				root = nullptr;
				return;
			}

			if (comparator(removedNode->value, st.top()->value)) {
				st.top()->left = nullptr;
			}
			else {
				st.top()->right = nullptr;
			}

			delete removedNode;
			removedNode = nullptr;


			reballanceStack(st);

		}
		else if (removedNode->right == nullptr) {

			if (removedNode == root) {
				root = removedNode->left;
				delete removedNode;
				root = ballance(root);
				return;
			}

			Node* next = removedNode->left;

			if (comparator(removedNode->value, st.top()->value)) {
				st.top()->left = next;
			}
			else {
				st.top()->right = next;
			}
			delete removedNode;
			removedNode = nullptr;

			reballanceStack(st);
		}
		else if (removedNode->left == nullptr) {

			if (removedNode == root) {
				root = removedNode->right;
				delete removedNode;
				root = ballance(root);
				return;
			}

			Node* next = removedNode->right;

			if (comparator(removedNode->value, st.top()->value)) {
				st.top()->left = next;
			}
			else {
				st.top()->right = next;
			}
			delete removedNode;
			removedNode = nullptr;

			reballanceStack(st);
		}
		else {

			Node* next = removedNode->right;

			st.push(removedNode);

			while (next->left) {
				st.push(next);
				next = next->left;
			}

			removedNode->value = next->value;

			if (comparator(next->value, st.top()->value)) {
				st.top()->left = next->right;
			}
			else {
				st.top()->right = next->right;
			}

			delete next;
			next = nullptr;


			reballanceStack(st);
		}
	}

	size_t size() {
		return size_;
	}

	T getK(size_t k) {
		if (k >= size_) {
			throw std::out_of_range("k >= root size");
		}

		Node* cur = root;
		size_t kk = weight(root->left);

		while (kk != k && cur != nullptr) {
			if (kk > k) {
				cur = cur->left;
				kk -= (weight(cur->right) + 1);
			}
			else {
				cur = cur->right;
				kk += weight(cur->left) + 1;
			}
		}

		if (cur == nullptr) {
			return 0;
		}

		return cur->value;
	}

	~avl_tree() {

		if (root == nullptr) {
			return;
		}

		std::queue<Node*> q;

		q.push(root);

		while (!q.empty()) {
			Node* cur = q.front();
			q.pop();

			if (cur) {
				if (cur->left) {
					q.push(cur->left);
				}
				if (cur->right) {
					q.push(cur->right);
				}

				delete cur;
				cur = nullptr;
			}
		}
	}

private:

	size_t height(Node* node) {
		return (node ? (node->height) : (0));
	}

	size_t weight(Node* node) {
		return (node ? (node->weight) : (0));
	}

	int64_t getBallance(Node* node) {
		return height(node->left) - height(node->right);
	}

	void fixHeight(Node* node) {
		size_t rightHeight = height(node->right);
		size_t leftHeight = height(node->left);

		node->height = ((rightHeight > leftHeight) ? rightHeight : leftHeight) + 1;
	}

	void fixWeight(Node* node) {
		size_t leftWeight = weight(node->left);
		size_t rightWeight = weight(node->right);

		node->weight = leftWeight + rightWeight + 1;
	}

	Node* smallRightRotation(Node* node) {
		Node* res = node->left;
		node->left = res->right;
		res->right = node;

		fixHeight(node);
		fixHeight(res);

		fixWeight(node);
		fixWeight(res);

		return res;
	}

	Node* smallLeftRotation(Node* node) {
		Node* res = node->right;
		node->right = res->left;
		res->left = node;

		fixHeight(node);
		fixHeight(res);

		fixWeight(node);
		fixWeight(res);

		return res;
	}

	Node* bigRightRotation(Node* node) {
		node->left = smallLeftRotation(node->left);
		return smallRightRotation(node);
	}

	Node* bigLeftRotation(Node* node) {
		node->right = smallRightRotation(node->right);
		return smallLeftRotation(node);
	}

	Node* exists_(T val, std::stack<Node*>& st) {

		st = std::stack<Node*>();

		if (root == nullptr) {
			return nullptr;
		}

		Node* cur = root;

		while (cur != nullptr && cur->value != val) {
			st.push(cur);

			if (comparator(val, cur->value)) {
				cur = cur->left;
			}
			else {
				cur = cur->right;
			}
		}

		if (cur == nullptr) {
			while (!st.empty()) {
				st.pop();
			}
		}

		return cur;
	}

	void reballanceStack(std::stack<Node*>& st) {
		if (st.empty()) {
			return;
		}

		Node* sub = st.top();
		sub = ballance(sub);
		st.pop();

		while (!st.empty()) {
			Node* par = st.top();
			st.pop();

			if (sub) {
				if (comparator(sub->value, par->value)) {
					par->left = sub;
				}
				else {
					par->right = sub;
				}
			}

			sub = ballance(par);
		}

		root = sub;
	}

	// Ballance = Left - Right
	// Ballance == 2
	// Left > Right

	// Ballance == -2
	// Left < Right

	Node* ballance(Node* node) {

		if (node == nullptr) {
			return nullptr;
		}

		fixHeight(node);

		fixWeight(node);

		// Правое вращение
		if (getBallance(node) > 1) {
			// Малое правое
			if (getBallance(node->left) >= 0) {
				return  smallRightRotation(node);
			}
			// Большое правое
			else {
				return bigRightRotation(node);
			}
		}

		// Левое вращение
		else if (getBallance(node) < -1) {
			// Малое левое
			if (getBallance(node->right) <= 0) {
				return smallLeftRotation(node);
			}
			// Большое левое
			else {
				return bigLeftRotation(node);
			}
		}

		return node;
	}

	size_t size_{};

	cmp comparator;
	Node* root{};
};
//////////////////////////////////////////////

int main() {

	int n{};

	int val{}, k{};

	avl_tree<int> tree;

	std::cin >> n;

	for (int i = 0; i < n; ++i) {
		std::cin >> val >> k;
		if (val >= 0) {
			tree.insert(val);
		}
		else {
			tree.remove(-val);
		}
		std::cout << tree.getK(k) << std::endl;
	}

	return 0;
}
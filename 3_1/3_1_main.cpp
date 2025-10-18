#include <iostream>

class annularQueue {
public:
	annularQueue() {}

	int pop_front() {
		if (is_empty()) {
			return -1;
		}

		int data = array[head];
		array[head] = 0;
		head = (++head) % capacity;
		--size;

		return data;
	}

	void push_back(int data) {
		if (size == capacity) {
			grow();
		};

		tail = (++tail) % capacity;
		array[tail] = data;
		++size;
	}

	bool is_empty() const {
		return size == 0;
	}

	~annularQueue() {
		delete[] array;
	}

private:

	void grow() {
		int newCapacity = (capacity * 2 > initialSize) ? (capacity * 2) : (initialSize);
		int* newArray = new int[newCapacity] {};

		for (int i = 0; i < size; ++i) {
			newArray[i] = array[(head + i) % capacity];
		}

		head = 0;
		tail = size - 1;

		delete[] array;
		array = newArray;
		capacity = newCapacity;
	}

	int* array{};
	int head{}, tail{};
	int size{};
	int capacity{};
	const int initialSize = 2;
};

void checkout(annularQueue& queue) {
	int n{};
	std::cin >> n;

	for (int i = 0; i < n; ++i) {
		int operation{}, value{};
		std::cin >> operation >> value;

		switch (operation) {
		case 2: {
			if (value != queue.pop_front()) {
				std::cout << "NO";
				return;
			}
			break;
		}

		case 3: {
			queue.push_back(value);
			break;
		}
		}
	}

	std::cout << "YES";
}

int main() {

	annularQueue queue;

	checkout(queue);

	return 0;
}
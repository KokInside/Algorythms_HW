#include <iostream>

/*
Реализовать очередь с динамическим зацикленным буфером.

Обрабатывать команды push back и pop front.

Формат ввода
В первой строке количество команд
n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 2 - pop front
a = 3 - push back

Если дана команда pop front, то число b - ожидаемое значение. 
Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.

Формат вывода	
Требуется напечатать YES - если все ожидаемые значения совпали. 
Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
*/

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
#include <iostream>
#include <stdexcept>

/*
4.3. Планировщик процессов

В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:

- приоритетом P
- временем, которое он уже отработал t
- временем, которое необходимо для завершения работы процесса T

Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
выполняет его время P и кладет обратно в очередь процессов.

Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.

Требования:

- В качестве очереди с приоритетом нужно использовать кучу.
- Куча должна быть реализована в виде шаблонного класса.
- Решение должно поддерживать передачу функции сравнения снаружи.
- Куча должна быть динамической.

Формат ввода
Сначала вводится кол-во процессов. После этого процессы в формате P T

Формат вывода
Кол-во переключений процессора.
*/

// структура процесса
struct process {
	process(const int& init_P, const int& init_T) : P(init_P), T(init_T) {}
	process() {}

	int P{};
	int t{};
	int T{};

	// метод выполнения процесса
	void perform() {
		t += P;
	}

};

// шаблонный класс саморасширяющегося динамического массива
template <typename T>
class dynamicArray {
public:
	dynamicArray() {}

	dynamicArray(int size) : _size(size), _capacity(size) {}

	void push_back(const T& val) {
		if (_size == _capacity) {
			grow();
		}
		array[_size++] = val;
	}

	T pop_back() {
		if (_size == 0) {
			throw std::out_of_range("out of dynamicArray range");
			return T();
		}
		return array[--_size];
	}

	// не знаю, можно ли пользоваться std::move
	void swap(T& left_val, T& right_val) {
		T temp = left_val;
		left_val = right_val;
		right_val = temp;
	}

	// оператор индексирования
	T& operator[] (const int& index) const {
		if (index >= _size || index < 0) {
			throw std::out_of_range("out of range");
		}
		return array[index];
	}

	int size() const {
		return _size;
	}

	~dynamicArray() {
		delete[] array;
	}

private:
	// расширение массива при привышении capacity
	void grow() {
		_capacity = (_size * 2 > initialSize) ? (_size * 2) : (initialSize);
		T* newArray = new T[_capacity];

		for (int i = 0; i < _size; ++i) {
			newArray[i] = array[i];
		}

		delete[] array;
		array = newArray;
	}

	T* array{};
	int _capacity{};
	int _size{};
	const int initialSize = 2;
};

// шаблонный класс кучи
template <typename T, typename Comparator>
class myHeap {
public:

	// посмотреть верхний элемент
	T top() const {
		if (array.size() != 0) {
			return array[0];
		}
	}

	// посмотреть верхний элемент и извлечением
	T extract_top() {
		if (array.size() == 0) {
			throw std::out_of_range("out of heap range");
			return T();
		}

		T res = array[0];

		if (array.size() == 1) {
			array.pop_back();
			return res;
		}

		array[0] = array.pop_back();
		sift_down();

		return res;
	}

	// вставить элемент
	void insert(T val) {
		array.push_back(val);

		if (array.size() == 1) {
			return;
		}

		sift_up(array.size() - 1);
	}

	int size() const {
		return array.size();
	}

private:

	void sift_up(int index) {
		if (index == 0) {
			return;
		}

		while (index != 0 && cmp(array[index], array[parent(index)])) {
			array.swap(array[index], array[parent(index)]);
			index = parent(index);
		}
	}

	void sift_down(int index = 0) {

		int left{};
		int right{};

		// реализовать====================================================================================================
		while (left != -1) {
			int left = left_child(index);
			int right = right_child(index);

			if (left == -1) {
				break;
			}

			int j = left;

			if (right != -1 && cmp(array[right], array[index])) {
				j = right;
			}

			if (cmp(array[index], array[j])) {
				break;
			}
			array.swap(array[index], array[j]);
			index = j;
		}
	}

	// индекс родятеля
	int parent(const int& index) {
		if (index == 0) {
			return -1;
		}
		return (index - 1) / 2;
	}

	// индекс левого потомка
	int left_child(const int& index) {
		if ((2 * index + 1) >= array.size()) {
			return -1;
		}
		return 2 * index + 1;
	}

	// индекс правого потомка
	int right_child(const int& index) {
		if ((2 * index + 2) >= array.size()) {
			return -1;
		}
		return 2 * index + 2;
	}

	dynamicArray<T> array;
	Comparator cmp;
};

// шаблонный класс компаратора
template <typename T>
class comparator {
public:
	comparator() {}

	bool operator() (const T& pr1, const T& pr2) const {
		return ((pr1.P * (pr1.t + 1)) < (pr2.P * (pr2.t + 1)));
	}

};


int main() {

	myHeap<process, comparator<process>> heap;

	int n{};

	std::cin >> n;

	// создание кучи
	for (int i = 0; i < n; ++i) {
		int P{}, T{};
		std::cin >> P >> T;
		heap.insert(process(P, T));
	}

	int count{};

	// повторять, пока куча не будет пустой
	while (heap.size() != 0) {

		// процесс выполнился - удалять его
		if (heap.top().t >= heap.top().T) {
			heap.extract_top();
			continue;
		}

		// все процессы выполнились (куча пустая)
		if (heap.size() == 0) {
			break;
		}
		
		// извлечение процесса
		process top = heap.extract_top();

		// выполнение извлечённого процесса
		top.perform();

		// вставка извлечённого процесса обратно
		heap.insert(top);

		// переключение процессора
		++count;
	}

	std::cout << count;

	return 0;
}
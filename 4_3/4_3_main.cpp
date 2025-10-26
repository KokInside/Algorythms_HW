#include <iostream>
#include <stdexcept>
#include <assert.h>

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

// шаблонный swap
// не знаю, можно ли использовать std::move
template<typename T>
void swap(T& pr1, T& pr2) {
	T tmp = pr1;
	pr1 = pr2;
	pr2 = tmp;
}

// структура процесса
struct process {
	process(int init_P, int init_T) : P(init_P), T(init_T) {}
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
	// по умолчанию
	dynamicArray() {}

	// размера size
	dynamicArray(size_t size) {
		size_ = size;
		capacity_ = size;
		array = new T[size];
	}

	// копирования
	dynamicArray(const dynamicArray& other) : capacity_(other.capacity_), size_(other.size_), array(new T[other.capacity_]) {
		for (size_t i = 0; i < size_; ++i) {
			array[i] = other.array[i];
		}
	}

	// корирование =
	dynamicArray& operator= (const dynamicArray& other) {
		if (this != &other) {
			delete[] array;
			this->size_ = other.size_;
			this->capacity_ = other.capacity_;
			array = new T[this->capacity_];
			for (size_t i = 0; i < this->size_; ++i) {
				this->array[i] = other.array[i];
			}
		}
		return *this;
	}

	// перемещения
	dynamicArray(dynamicArray&& other) : size_(other.size_), capacity_(other.capacity_), array(other.array) {
		other.array = nullptr;
		other.size_ = other.capacity_ = 0;
	}

	// перемещения =
	dynamicArray& operator= (dynamicArray&& other) {
		if (this != &other) {
			delete[] array;
			array = other.array;
			size_ = other.size_;
			capacity_ = other.capacity_;
			other.array = nullptr;
			other.size_ = other.capacity_ = 0;
		}
		return *this;
	}

	void push_back(const T& val) {
		if (size_ == capacity_) {
			grow();
		}
		array[size_++] = val;
	}

	T& back() const {
		if (size_ <= 0) {
			std::abort();
		}
		return array[size_ - 1];
	}

	void pop_back() {
		if (size_ == 0) {
			throw std::out_of_range("pop back on empty array");
		}
		--size_;
	}

	const T& operator[] (const size_t index) const {
		if (index >= size_ || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}

	T& operator[] (const size_t index) {
		if (index >= size_ || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}

	size_t size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	size_t capacity() const {
		return capacity_;
	}

	void reserve(size_t newCapacity) {

		grow(newCapacity);
	}

	void clear() {
		size_ = 0;
	}

	void resize(size_t newSize) {
		if (newSize > capacity_) {
			grow(newSize);
		}
		size_ = newSize;
	}

	void shrink_to_fit() {
		if (size_ < capacity_) {
			grow(size_);
		}
	}

	T* begin() {
		return array;
	}

	T* end() {
		return array + size_;
	}

	const T* begin() const {
		return array;
	}

	const T* end() const {
		return array + size_;
	}

	const T* cbegin() const {
		return array;
	}

	const T* cend() const {
		return array + size_;
	}

	~dynamicArray() {
		delete[] array;
		array = nullptr;
	}

private:

	void grow(size_t newCapacity = 0) {
		if (newCapacity < 0) {
			throw std::invalid_argument("Negative capacity");
		}

		size_t targetCapacity = newCapacity ? newCapacity : (size_ * 2 > initialSize ? size_ * 2 : initialSize);

		if (targetCapacity <= capacity_) {
			return;
		}

		assert(size_ <= targetCapacity && "Size should not exceed target capacity");

		T* newArray = new T[targetCapacity];

		for (size_t i = 0; i < size_; ++i) {
			newArray[i] = array[i];
		}

		delete[] array;
		array = newArray;
		capacity_ = targetCapacity;
	}

	T* array = nullptr;
	size_t capacity_{};
	size_t size_{};
	static constexpr size_t initialSize = 2;
};

// шаблонный класс кучи
template <typename T, typename Comparator>
class myHeap {
public:

	myHeap() {}

	myHeap(size_t reserveCapacity) {
		array.reserve(reserveCapacity);
	}

	// посмотреть верхний элемент
	const T& top() const {
		if (array.size() != 0) {
			return array[0];
		}
		throw std::out_of_range("empty heap");
	}

	// посмотреть верхний элемент и извлечением
	T extract_top() {
		if (array.size() == 0) {
			throw std::out_of_range("out of heap range");
		}

		T res = array[0];

		if (array.size() == 1) {
			array.pop_back();
			return res;
		}

		array[0] = array.back();
		array.pop_back();
		sift_down();

		return res;
	}

	// вставить элемент
	void insert(T val) {
		array.push_back(val);

		if (array.size() <= 1) {
			return;
		}

		sift_up(array.size() - 1);
	}

	size_t size() const {
		return array.size();
	}

	bool empty() const {
		return array.size() == 0;
	}

private:

	void sift_up(int index) {
		if (index == 0) {
			return;
		}

		while (index != 0 && cmp(array[index], array[parent(index)])) {
			swap(array[index], array[parent(index)]);
			index = parent(index);
		}
	}

	void sift_down(int index = 0) {

		int left{};
		int right{};

		while (left != -1) {
			left = left_child(index);
			right = right_child(index);

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
			swap(array[index], array[j]);
			index = j;
		}
	}

	// индекс родятеля
	int parent(int index) {
		if (index == 0) {
			return -1;
		}
		return (index - 1) / 2;
	}

	// индекс левого потомка
	int left_child(int index) {
		if ((2 * index + 1) >= array.size()) {
			return -1;
		}
		return 2 * index + 1;
	}

	// индекс правого потомка
	int right_child(int index) {
		if ((2 * index + 2) >= array.size()) {
			return -1;
		}
		return 2 * index + 2;
	}

	dynamicArray<T> array;
	Comparator cmp;
};

// класс компаратора
class comparator {
public:
	comparator() {}

	bool operator() (const process& pr1, const process& pr2) const {
		return ((pr1.P * (pr1.t + 1)) < (pr2.P * (pr2.t + 1)));
	}

};


int main() {

	int n{};

	std::cin >> n;

	myHeap<process, comparator> heap(n);

	// заполняем кучу
	for (int i = 0; i < n; ++i) {
		int P{}, T{};
		std::cin >> P >> T;
		heap.insert(process(P, T));
	}

	int count{};

	// повторять, пока куча не будет пустой
	while (!heap.empty()) {

		// процесс выполнился - удалять его
		if (heap.top().t >= heap.top().T) {
			heap.extract_top();
			continue;
		}

		// все процессы выполнились (куча пустая) - можно выходить
		if (heap.empty()) {
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
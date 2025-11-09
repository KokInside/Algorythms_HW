#include <iostream>
#include <ctime>
#include <cstdlib>
#include <assert.h>

/*
6.4. Порядковая статистика и параметры множества

Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
- 10%  перцентиль
- медиана
- 90%  перцентиль

Требования: к дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

// функция возвращает случайное число [from; to)
int random(int from, int to) {
	return from + rand() % (to - from);
}

// шаблонный swap
template<typename T>
void swap(T& pr1, T& pr2) {
	T tmp = pr1;
	pr1 = pr2;
	pr2 = tmp;
}

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

	// корирования =
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
		if (size_ == 0) {
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
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return array[index];
	}

	T& operator[] (const size_t index) {
		if (index >= size_) {
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

	T* data() {
		return array;
	}

	const T* data() const {
		return array;
	}

	~dynamicArray() {
		delete[] array;
		array = nullptr;
	}

private:

	void grow(size_t newCapacity = 0) {

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

// выбор опорного элемента
template <typename T, typename Comparator>
T* partition(T* begin, T* end, Comparator cmp = Comparator()) {

	// pivot == [begin; end)
	T* pivot = begin + random(0, (end - begin));

	// перемещение pivot в начало
	swap(*pivot, *(begin));
	pivot = begin;

	// 2 указателя в конец
	T* i = end - 1;
	T* j = end - 1;

	while (j > begin) {

		if (cmp(*pivot, *j)) {
			swap(*i, *j);
			--i;
			--j;
		}
		else {
			--j;
		}
	}

	swap(*i, *begin);

	return i;
}

// шаблонная функция поиска k-ой порядковой статистики
template <typename T, typename Comparator>
T* k_statistics(T* begin, T* end, int order, Comparator cmp = Comparator()) {

	T* pivot_pos = partition(begin, end, cmp);
	T* left = begin;
	T* right = end;

	while ((begin + order) != pivot_pos) {

		if (pivot_pos < (begin + order)) {
			left = pivot_pos;
			pivot_pos = partition(left, right, cmp);
		}

		else {
			right = pivot_pos;
			pivot_pos = partition(left, right, cmp);
		}
	}

	return pivot_pos;
}

// Шаблонный класс компаратора
template <typename T>
class comparator {
public:
	comparator() {}

	const bool operator() (const T& a, const T& b) {
		return a < b;
	}
};

int main() {

	srand(time(NULL));

	int n{};
	std::cin >> n;

	dynamicArray<int> arr(n);

	for (int i = 0; i < n; ++i) {
		std::cin >> arr[i];
	}

	std::cout << *k_statistics(arr.begin(), arr.end(), static_cast<int>(arr.size() * 0.1), comparator<int>()) << std::endl;

	std::cout << *k_statistics(arr.begin(), arr.end(), static_cast<int>(arr.size() / 2), comparator<int>()) << std::endl;

	std::cout << *k_statistics(arr.begin(), arr.end(), static_cast<int>(arr.size() * 0.9), comparator<int>());

	return 0;
}
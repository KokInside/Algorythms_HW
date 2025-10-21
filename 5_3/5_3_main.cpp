#include <iostream>
#include <stdexcept>
#include <assert.h>

/*
5.3. «акраска пр€мой

Ќа числовой пр€мой окрасили N отрезков.
»звестны координаты левого и правого концов каждого отрезка [L_i, R_i].
Ќайти длину окрашенной части числовой пр€мой.

N <= 10000. L_i, R_i Ч целые числа в диапазоне [0, 10^9].

‘ормат ввода
¬ первой строке записано количество отрезков.
¬ каждой последующей строке через пробел записаны координаты левого и правого концов отрезка.

‘ормат вывода
¬ыведите целое число Ч длину окрашенной части.

*/

// шаблонный swap
template<typename T>
void swap(T& pr1, T& pr2) {
	T tmp = pr1;
	pr1 = pr2;
	pr2 = tmp;
}

// шаблонный класс саморасшир€ющегос€ динамического массива
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

	// копировани€
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

	// перемещени€
	dynamicArray(dynamicArray&& other) : size_(other.size_), capacity_(other.capacity_), array(other.array) {
		other.array = nullptr;
		other.size_ = other.capacity_ = 0;
	}

	// перемещени€ =
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

// структура интервала
struct interval {
	interval() {}
	interval(int init_point, int init_thicknessDelta) : point(init_point), thicknessDelta(init_thicknessDelta) {}

	unsigned int point{};
	int thicknessDelta{};
};

// шаблонна€ функци€ сортировки сли€нием
template <typename T, typename Comparator>
void mergeSort(dynamicArray<T>& array, int start, int end, Comparator cmp) {
	if (end - start <= 1) {
		return;
	}

	int mid = (start + end) / 2;

	mergeSort(array, start, mid, cmp);
	mergeSort(array, mid, end, cmp);

	dynamicArray<T> buffer;
	buffer.reserve(end - start);

	int lidx = start, ridx = mid;

	while (lidx < mid && ridx < end) {
		if (cmp(array[lidx], array[ridx])) {
			buffer.push_back(array[lidx++]);
		}
		else {
			buffer.push_back(array[ridx++]);
		}
	}

	if (lidx != mid) {
		for (lidx; lidx < mid; ++lidx) {
			buffer.push_back(array[lidx]);
		}
	}
	else if (ridx != end) {
		for (ridx; ridx < end; ++ridx) {
			buffer.push_back(array[ridx]);
		}
	}

	while (lidx < mid) {
		buffer.push_back(array[lidx++]);
	}
	while (ridx < end) {
		buffer.push_back(array[ridx++]);
	}

	for (int i = start, j = 0; i < end; ++i, ++j) {
		array[i] = buffer[j];
	}
}

// шаблонный класс компаратора
template <typename T>
class comparator {
public:
	comparator() {}

	bool operator() (const T& pr1, const T& pr2) const {
		return (pr1 <= pr2);
	}
};

int main() {

	dynamicArray<interval> arr;
	
	int n{};
	int startPoint{}, endPoint{};
	
	std::cin >> n;
	
	arr.reserve(n * 2);
	
	// заполн€ем массив
	for (int i = 0; i < n; ++i) {
		std::cin >> startPoint >> endPoint;
		arr.push_back(interval(startPoint, 1));
		arr.push_back(interval(endPoint, -1));
	}
	
	// пересобрать на итераторы
	mergeSort(arr, 0, arr.size(), [](interval a, interval b) { return a.point < b.point; });
	
	if (arr.empty()) {
		std::cout << 0;
		return 0;
	}
	
	int prev = arr[0].point;
	
	int res{};
	
	int cur_thickness{};
	
	for (int i = 0; i < arr.size(); ++i) {
		if (cur_thickness > 0) {
			res += arr[i].point - prev;
		}
		cur_thickness += arr[i].thicknessDelta;
		prev = arr[i].point;
	}
	
	std::cout << res;

	return 0;
}	
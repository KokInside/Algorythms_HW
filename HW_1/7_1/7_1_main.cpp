#include <iostream>
#include <string>
#include <assert.h>

/*
7.1. MSD для строк

Дан массив строк. Количество строк не больше 100000.
Отсортировать массив методом поразрядной сортировки MSD по символам.
Размер алфавита - 256 символов. Последний символ строки = '\0'.
*/

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
		array = new T[size]{};
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

	const size_t size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	const size_t capacity() const {
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

	void reset() {
		for (int i = 0; i < size_; ++i) {
			array[i] = T();
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

// возвращает индекс строки по индексу pos. Предотвращает выход за пределы строки
static int getKey(const std::string& str, int pos) {
	if (pos >= static_cast<unsigned int>(str.size())) {
		return 0;
	}

	return static_cast<unsigned int>(str[pos]);
}

// функция порязрядной сортировки
static void msd(const dynamicArray<std::string>& arr, int begin, int end, dynamicArray<int>& idx, int pos) {

	// одна строка для данного разряда - сортировать не нужно
	if (end - begin <= 1) {
		return;
	}

	// нахождение минимального и максимального значения для данного разряда
	int min = getKey(arr[idx[begin]], pos), max = getKey(arr[idx[begin]], pos);

	for (int i = begin; i < end; ++i) {
		int cur = getKey(arr[idx[i]], pos);
		if (cur > max) {
			max = cur;
		}
		if (cur < min) {
			min = cur;
		}
	}

	// если разряд пустой (все символы в нём == '\0') - return
	if (max == 0 && min == 0) {
		return;
	}

	// count - массив с размером задействованного алфавита + 1, чтобы добавить одно значение в конце
	dynamicArray<int> count(max - min + 2);

	// здесь count будет содержать количество строк с символом в данном разряде
	for (int i = begin; i < end; ++i) {
		++count[getKey(arr[idx[i]], pos) - min];
	}

	// здесь count будет содержать индексы последней позиции строк с символом текущего разряда
	for (int i = 1; i < max - min + 1; ++i) {
		count[i] += count[i - 1];
	}

	// СФОРМИРОВАН МАССИВ COUNT С ИНДЕКСАМИ КОНЦОВ СИМВОЛОВ

	// внутри idx работаем только в диапазоне [begin; end), вылезать за него нельзя

	// tmp - временный массив, в который будет вставляться отсортированные строки
	dynamicArray<int> tmp(end - begin);

	// идём с конца массива arr, вставляем в tmp строки из arr, порядок получается отсортированным в пределах разряда
	for (int i = end - 1; i >= begin; --i) {
		tmp[--count[getKey(arr[idx[i]], pos) - min]] = idx[i];
	}

	// помещает отсортированные значения из временного массива tmp в массив idx
	for (int i = begin; i < end; ++i) {
		idx[i] = tmp[i - begin];
	}

	// для удобства в последний элемент count помещаю размер сортируемого массива. Чтобы последний участок сортировки в пределах одного разряда
	// было удобно передавать дальше на сортировку
	count[max - min + 1] = end - begin;

	// Запускаем рекурсию сортировки внутри разряда с одинаковыми символами
	for (int i = 0; i < max - min + 1; ++i) {
		// условие не пуского разряда
		if (count[i] == count[i + 1]) {
			continue;
		}
		msd(arr, count[i] + begin, count[i + 1] + begin, idx, pos + 1);
	}
}

// Глубина рекурсии ограничена максимальным размером передаваемой строки

int main() {

	dynamicArray<std::string> arr;
	std::string str;

	int n{};

	while (std::cin >> str) {
		++n;
		arr.push_back(std::move(str));
	}

	// Чтобы не передвигать строки в массиве arr при сортировке, лучше хранить индексы в массиве idx, ссылающиеся на индекс строки в arr.
	// Значение в idx - индекс строки в arr
	// Значения строк в idx хранятся в отсортированном порядке
	dynamicArray<int> idx(n);

	for (int i = 0; i < n; ++i) {
		idx[i] = i;
	}

	msd(arr, 0, n, idx, 0);

	for (int i = 0; i < n; ++i) {
		std::cout << arr[idx[i]] << std::endl;
	}

	return 0;
}
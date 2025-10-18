#include <iostream>

/*
2.2
Дан массив целых чисел А[0..n-1].
Известно, что на интервале [0, m] значения массива строго возрастают,
а на интервале [m, n-1] строго убывают.
Найти m за O( log m ).
2 ≤ n ≤ 10000.
*/

// Бинарный поиск "разворота". Основан на дельте между соседними элементами
int binary_search(int*& mass, int start, int end) {
	while (start < end) {
		int mid = (start + end) / 2;
		if ((mass[mid] - mass[mid - 1]) > 0 && (mass[mid + 1] - mass[mid]) > 0) {
			start = mid + 1;
		}
		else {
			end = mid;
		}
	}
	return start;
}

// Экспоненциальный поиск интервала с "разворотом". 
// Основан на дельте соседних элементов
void exponential_search(int*& mass, int size, int& start, int& end) {

	start = 0, end = 1;

	while ((mass[start + 1] - mass[start]) > 0 && (mass[end] - mass[end - 1]) > 0) {
		start = end;
		end *= 2;
		// если вышли за пределы
		if (end > size - 2) {
			end = size - 2;
			break;
		}
	}
	// чтобы не выйти за пределы
	// нужно инкрементировать end, чтобы end не мог быть индексом разворота
	if (end != size - 1) {
		++end;
	}

}

int main() {

	int n{};

	std::cin >> n;

	int* mass = new int[n];

	for (int i = 0; i < n; ++i) {
		std::cin >> mass[i];
	}
	int start{}, end{};

	exponential_search(mass, n, start, end);

	std::cout << binary_search(mass, start, end);

	delete[] mass;
	return 0;
}
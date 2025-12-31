#include <iostream>
#include "4_header.h"
#include <array>
#include <unordered_map>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>

/*

Написать алгоритм для решения игры в “пятнашки”. 
Решением задачи является приведение к виду: 
[ 1 2 3 4 ]
[ 5 6 7 8 ] 
[ 9 10 11 12] 
[ 13 14 15 0 ], 
где 0 задает пустую ячейку. 

Достаточно найти хотя бы какое-то решение. 
Число перемещений костяшек не обязано быть минимальным.

*/

int main() {

	std::array<uint8_t, 16> arr{};

	int a{};

	for (int i = 0; i < 16; ++i) {
		std::cin >> a;
		arr[i] = a;
	}

	std::string str = GetSolution(arr);

	if (str == "-1") {
		std::cout << str;
	}
	else {
		std::cout << str.size() << std::endl << str;
	}

	return 0;
}
#include <iostream>
#include "4_header.h"
#include <array>
#include <unordered_map>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>

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
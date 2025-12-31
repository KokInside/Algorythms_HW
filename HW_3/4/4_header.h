#pragma once
#include <array>
#include <unordered_map>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <stdexcept>

const static std::array<uint8_t, 16> finish = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };

// класс состояния
class State {
public:

	State(): emptyPos(-1) {

	}

	State(std::array<uint8_t, 16> arr) {
		array = std::move(arr);

		for (int i = 0; i < size; ++i) {
			if (arr[i] == 0) {
				emptyPos = i;
				break;
			}
		}
	}

	bool isComplete() const {
		return array == finish;
	}

	bool isSolvable() const {
		int A = getInvCount();

		int B = (emptyPos / 4) + 1;

		return (A + B) % 2 == 0;
	}

	// эвристика по минимальному количеству перемещений костяшки на нужное место (как на семинаре)
	int getHeuristic() const {

		int heuristic{};

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				int val = this->array[i * 4 + j];

				if (val == 0) {
					continue;
				}

				int trueRow = (val - 1) / 4;
				int trueCol = (val - 1) % 4;

				heuristic += std::abs(i - trueRow);
				heuristic += std::abs(j - trueCol);
			}
		}

		return heuristic;
	}

	bool operator == (const State& other) const {
		return array == other.array;
	}


	bool canMoveRight() const {
		return emptyPos % 4 != 0;
	}

	bool canMoveLeft() const {
		return emptyPos % 4 != 3;
	}

	bool canMoveUp() const {
		return emptyPos < 12;
	}

	bool canMoveDown() const {
		return emptyPos > 3;
	}


	State MoveRight() const {

		if (!canMoveRight()) {
			throw std::logic_error("Can't move right");
		}

		State newState(*this);

		std::swap(newState.array[emptyPos], newState.array[emptyPos - 1]);

		--newState.emptyPos;
		return newState;

	}

	State MoveLeft() const {

		if (!canMoveLeft()) {
			throw std::logic_error("Can't move left");
		}

		State newState(*this);

		std::swap(newState.array[emptyPos], newState.array[emptyPos + 1]);

		++newState.emptyPos;
		return newState;
	}

	State MoveUp() const {

		if (!canMoveUp()) {
			throw std::logic_error("Can't move up");
		}

		State newState(*this);

		std::swap(newState.array[emptyPos], newState.array[emptyPos + 4]);

		newState.emptyPos += 4;
		return newState;
	}

	State MoveDown() const {

		if (!canMoveDown()) {
			throw std::logic_error("Can't move down");
		}

		State newState(*this);

		std::swap(newState.array[emptyPos], newState.array[emptyPos - 4]);

		newState.emptyPos -= 4;
		return newState;
	}


	static const int size = 16;

	friend class Hasher;

	friend std::string GetSolution(const std::array<uint8_t, 16>& arr);

private:

	// количество инверсий костяшек
	int getInvCount() const {
		int invCount{};

		for (int i = 0; i < size - 1; ++i) {
			for (int j = i + 1; j < size; ++j) {
				if ((array[i] > array[j]) && array[i] && array[j]) {
					++invCount;
				}
			}
		}

		return invCount;
	}

	std::array<uint8_t, 16> array{};

	int emptyPos = -1;
};

// хэширование состояния
class Hasher {
public:

	size_t operator() (const State& state) const {
		size_t hash{};

		size_t p = 3;

		for (int i = 0; i < State::size; ++i) {
			hash += p * hash + state.array[i];
		}

		return hash;
	}
};

// сравнивание состояний по эвристикам
class CMP {
public:

	bool operator() (const State& a, const State& b) {
		return a.getHeuristic() > b.getHeuristic();
	}
};

std::string GetSolution(const std::array<uint8_t, 16>& arr) {
	
	State start(arr);

	if (!start.isSolvable()) {
		return "-1";
	}

	std::unordered_map<State, char, Hasher> visited;

	visited[start] = 'S';

	// приоритет по наименьшей эвристике
	std::priority_queue<State, std::vector<State>, CMP> q;

	q.push(start);

	while (true) {

		State state = q.top();
		q.pop();

		if (state.isComplete()) {
			break;
		}

		// просто большое начальное значение эвристик
		int heuristic = 1000;
		int heuristic_2 = 1000;

		// будем рассматривать только 2 состояния с наименьшими эвристиками
		std::pair<State, char> first;
		std::pair<State, char> second;


		// 4 аналогичных раза пытаемся сдвигуть по 4-м направлениям. Запоминаем 2 наименьшие эвристики. 
		// 
		// в first будет точно наименьшая, потому что она будет всегда перезаписывать, если там лежит что-то больше
		// в second если повезёт что-то будет находиться, но только если хоть одна следующая эвристика будет больше любой предыдущей
		
		if (state.canMoveDown()) {
			State newState = state.MoveDown();

			if (visited.find(newState) == visited.end()) {
				if (newState.getHeuristic() < heuristic) {
					heuristic = newState.getHeuristic();
					first.first = std::move(newState);
					first.second = 'D';
				}
				else if (newState.getHeuristic() < heuristic_2) {
					heuristic_2 = newState.getHeuristic();
					second.first = std::move(newState);
					second.second = 'D';
				}
			}
		}

		if (state.canMoveUp()) {
			State newState = state.MoveUp();

			if (visited.find(newState) == visited.end()) {
				if (newState.getHeuristic() < heuristic) {
					heuristic = newState.getHeuristic();
					first.first = std::move(newState);
					first.second = 'U';
				}
				else if (newState.getHeuristic() < heuristic_2) {
					heuristic_2 = newState.getHeuristic();
					second.first = std::move(newState);
					second.second = 'U';
				}
			}
		}

		if (state.canMoveRight()) {
			State newState = state.MoveRight();

			if (visited.find(newState) == visited.end()) {
				if (newState.getHeuristic() < heuristic) {
					heuristic = newState.getHeuristic();
					first.first = std::move(newState);
					first.second = 'R';
				}
				else if (newState.getHeuristic() < heuristic_2) {
					heuristic_2 = newState.getHeuristic();
					second.first = std::move(newState);
					second.second = 'R';
				}
			}
		}

		if (state.canMoveLeft()) {
			State newState = state.MoveLeft();

			if (visited.find(newState) == visited.end()) {
				if (newState.getHeuristic() < heuristic) {
					heuristic = newState.getHeuristic();
					first.first = std::move(newState);
					first.second = 'L';
				}
				else if (newState.getHeuristic() < heuristic_2) {
					heuristic_2 = newState.getHeuristic();
					second.first = std::move(newState);
					second.second = 'L';
				}
			}
		}

		// пушим только 2 состояния с наименьшими эвристиками
		if (first.first.emptyPos != -1) {
			visited[first.first] = first.second;
			q.push(first.first);
		}
		if (second.first.emptyPos != -1) {
			visited[second.first] = second.second;
			q.push(second.first);
		}
	}

	std::string path{};

	State state(finish);

	while (visited[state] != 'S') {
		char move = visited[state];

		switch (move) {
		case 'L': {
			state = state.MoveRight();
			path += 'L';
			break;
		}
		case 'R': {
			state = state.MoveLeft();
			path += 'R';
			break;
		}
		case 'U': {
			state = state.MoveDown();
			path += 'U';
			break;
		}
		case 'D': {
			state = state.MoveUp();
			path += 'D';
			break;
		}
		}
	}

	std::reverse(path.begin(), path.end());

	return path;
}
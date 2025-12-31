#include <iostream>
#include <string>
#include <vector>

/*
1. Хеш-таблица

Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 2. Для разрешения коллизий используйте двойное хеширование.
*/

// структура хэшера - всегда нечётное
struct Hasher {
	Hasher(size_t p) : p_(p) {}

	size_t operator()(const std::string& key) const {
		size_t hash = 0;

		for (const char& c : key) {
			hash = hash * p_ + c;
		}

		return (hash * 2) + 1;
	}

private:
	size_t p_{};
};


// шаблонный класс хэш таблицы
template <typename T, typename H = Hasher>
class HashTable {
public:
	HashTable(H hasher = Hasher(113)) : hasher_(hasher), table(initial_size), size_(0), deleted_size(0) {}

	bool Add(const T& key) {
		// если нужно - делаем расширение
		if ((size_ + deleted_size) * 4 >= 3 * table.size()) {
			grow();
		}

		const size_t hash = hasher_(key);
		const size_t step = SH(key);

		return AddKey_(key, table, hash, step);
	}

	bool Delete(const T& key) {
		const size_t hash = hasher_(key);
		size_t index = hash % table.size();
		const size_t step = SH(key);

		for (int i = 0; i < table.size(); ++i) {
			// если пусто - нечего удалять
			if (table[index].status == Status::Empty) {
				return false;
			}
			// если удалён - ищем дальше

			// если элемент имеет ключ
			else if (table[index].status == Status::HasKey) {
				// если это нужный для удаления ключ
				if (table[index].key == key) {
					table[index].status = Status::Deleted;
					--size_;
					++deleted_size;
					return true;
				}
			}

			index = (index + step) % table.size();
		}

		return false;
	}

	bool Has(const T& key) const {
		const size_t hash = hasher_(key);
		size_t index = hash % table.size();
		const size_t step = SH(key);

		for (int i = 0; i < table.size(); ++i) {
			// если не нашли
			if (table[index].status == Status::Empty) {
				return false;
			}
			// если элемент удалён - продолжаем искать дальше

			// если элемент имеет ключ
			else if (table[index].status == Status::HasKey) {
				// если это наш ключ - нашли
				if (table[index].key == key) {
					return true;
				}
			}

			index = (index + step) % table.size();
		}

		return false;
	}

private:

	// хранит состояние узла
	enum class Status { HasKey, Deleted, Empty };

	static constexpr size_t initial_size = 8;

	H hasher_;

	// хранит хэш и шаг, чтобы не перевычислять каждый раз хэш элемента
	struct Node {
		size_t hash{};
		T key{};
		Status status = Status::Empty;
		size_t step{};
	};

	// основная "таблица"
	std::vector<Node> table{};

	size_t size_{};
	size_t deleted_size{};

	// структура второго хэша - всегда нечётное
	struct SecondHasher {
		size_t operator()(const T& key) const {
			size_t hash{};
			const size_t p = 137;

			for (const char& c : key) {
				hash = hash * p + c;
			}

			return (hash * 2) + 1;
		}
	};

	SecondHasher SH;

	// Приватная вставка с дополнительными служебными параметрами
	bool AddKey_(const T& key, std::vector<Node>& table, const size_t hash, const size_t step, bool increase_size = true) {
		size_t index = hash % table.size();
		size_t first_deleted{};
		bool has_first_deleted = false;

		for (int i = 0; i < table.size(); ++i) {
			// Если по хэшу ничего нет
			if (table[index].status == Status::Empty) {
				// если раньше встретили удалённый - можем вставить на его место
				if (has_first_deleted == true) {
					index = first_deleted;
				}

				// просто вставляем ключ на пустое место
				table[index].key = key;
				table[index].status = Status::HasKey;
				table[index].hash = hash;
				table[index].step = step;

				if (increase_size) {
					++size_;
				}

				return true;
			}
			// Если по хэшу что-то лежит
			else if (table[index].status == Status::HasKey) {
				// если лежит наш ключ - значит уже есть - вставлять не нужно
				if (table[index].key == key) {
					return false;
				}

				// если там не наш ключ - значит он может быть дальше - делаем шаг
			}
			// Если по хэшу удалённый элемент
			else if (table[index].status == Status::Deleted) {
				// если это первый встреченный удалённый - запоминаем его
				if (has_first_deleted == false) {
					has_first_deleted = true;
					first_deleted = index;
				}

				// наш ключ может быть дальше - делаем шаг
			}

			index = (index + step) % table.size();
		}

		if (has_first_deleted == true) {
			index = first_deleted;

			// просто вставляем ключ на пустое место
			table[index].key = key;
			table[index].status = Status::HasKey;
			table[index].hash = hash;
			table[index].step = step;

			if (increase_size) {
				++size_;
			}

			return true;
		}
		else {
			grow();
			return false;
		}

	}

	// увеличение + рехэширование. Размер всегда степень 2
	void grow() {
		// увеличиваем таблицу в 2 раза
		std::vector<Node> newTable(table.size() * 2);
		size_t newSize{};

		for (size_t i = 0; i < table.size(); ++i) {
			if (table[i].status == Status::HasKey) {

				// старые значения хэшей можно перенести, они не зависят от размеров таблицы
				bool inserted = AddKey_(table[i].key, newTable, table[i].hash, table[i].step, false);

				if (inserted == true) {
					++newSize;
				}
			}
		}

		size_ = newSize;
		deleted_size = 0;

		table = std::move(newTable);
	}
};


int main() {

	Hasher hasher(113);

	HashTable<std::string, Hasher> map(hasher);

	char c{};
	std::string str{};

	while (std::cin >> c >> str) {
		switch (c) {
		case '+': {
			std::cout << (map.Add(str) ? "OK" : "FAIL") << std::endl;
			break;
		}
		case '-': {
			std::cout << (map.Delete(str) ? "OK" : "FAIL") << std::endl;
			break;
		}
		case '?': {
			std::cout << (map.Has(str) ? "OK" : "FAIL") << std::endl;
			break;
		}
		}
	}

	return 0;
}
#include <iostream>
#include <vector>
#include <functional>
#include <stack>
#include <queue>
#include <utility>
#include <map>
#include <set>
#include <cstdint>
#include <algorithm>

/*
3. Города

Требуется отыскать самый короткий маршрут между городами.
Из города может выходить дорога, которая возвращается в этот же город.

Требуемое время работы O((N+M)logN), где
N – количество городов,
M – известных дорог между ними.

Граф должен быть реализован в виде класса.

*/

class IGraph {
public:

	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;

};

// Реализация Графа
class ListGraph : public IGraph {
public:

	ListGraph(size_t size);

	ListGraph(const IGraph& graph);

	void AddEdge(int from, int to, int weight) override;

	int VerticesCount() const override;

	std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;

	~ListGraph();

private:

	std::vector<std::vector<std::pair<int, int>>> adjacencyList;
};

ListGraph::ListGraph(size_t size) : adjacencyList(size) {
}

void ListGraph::AddEdge(int from, int to, int weight) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	if (from == to) {
		return;
	}

	auto it = std::find_if(adjacencyList[from].begin(), adjacencyList[from].end(), [&to](const std::pair<int, int>& p) {return p.first == to; });

	// Если ребро есть и оно легче - обновляем
	if (it != adjacencyList[from].end()) {
		if (weight < it->second) {
			it->second = weight;
		}
	}
	else {
		adjacencyList[from].push_back(std::pair<int, int>(to, weight));
	}

	// Если есть обратное ребро и оно легче - обновляем
	if (from != to) {
		auto it = std::find_if(adjacencyList[to].begin(), adjacencyList[to].end(), [&from](const std::pair<int, int>& p) {return p.first == from; });
		
		if (it != adjacencyList[to].end()) {
			if (weight < it->second) {
				it->second = weight;
			}
		}
		else {
			adjacencyList[to].push_back(std::pair<int, int>(from, weight));
		}
	}
}

int ListGraph::VerticesCount() const {

	return static_cast<int>(adjacencyList.size());
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	return adjacencyList[vertex];
}

ListGraph::~ListGraph() {
}

// Дейкстра
size_t ShortestPath(const IGraph& graph, int from, int to) {

	if (from < 0 || to < 0 || from >= graph.VerticesCount() || to >= graph.VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	std::vector<size_t> minPath(graph.VerticesCount(), UINT64_MAX);

	minPath[from] = 0;

	std::set<std::pair<size_t, size_t>> s;

	s.emplace(0, from);

	while (!s.empty()) {
		size_t vertex = (*(s.begin())).second;

		s.erase(s.begin());

		std::vector<std::pair<int, int>> nextVertices = graph.GetNextVertices(vertex);

		for (int i = 0; i < nextVertices.size(); ++i) {

			int path = nextVertices[i].second;
			int ver = nextVertices[i].first;

			if (minPath[ver] == UINT64_MAX || minPath[ver] > minPath[vertex] + path) {
				s.erase({ minPath[ver], ver });

				minPath[ver] = minPath[vertex] + path;

				s.emplace(minPath[ver], ver);
			}
		}
	}

	return minPath[to];
}

int main() {

	int N{}, M{};

	std::cin >> N >> M;

	ListGraph graph(N);

	for (int i = 0; i < M; ++i) {
		int u{}, v{}, w{};

		std::cin >> u >> v >> w;

		graph.AddEdge(u, v, w);
	}

	int s{}, t{};
	
	std::cin >> s >> t;

	std::cout << ShortestPath(graph, s, t);

	return 0;
}
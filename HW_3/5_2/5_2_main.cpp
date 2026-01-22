#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <numeric>
#include <cstdint>
#include <algorithm>

/*
5.2. Минимальное остовное дерево

Дан неориентированный связный граф. 
Требуется найти вес минимального остовного дерева в этом графе.

Вариант 2. С помощью алгоритма Прима.

*/

class IGraph {
public:

	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
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

// Spanning tree = Остовное дерево
void MinSpanningTree(const IGraph& graph, std::vector<bool>& visited, std::vector<int>& parent, std::vector<size_t>& minEdge) {

	// стартовая вершина 0

	minEdge[0] = 0;

	// <edge_weight, vertex>
	std::set<std::pair<int, int>> s;

	s.emplace(0, 0);

	while (!s.empty()) {
		int cur_vertex = (*s.begin()).second;

		visited[cur_vertex] = true;

		s.erase(s.begin());

		std::vector<std::pair<int, int>> nextVertices = graph.GetNextVertices(cur_vertex);

		// c++17
		for (auto& [vertex, edge] : nextVertices) {

			if (visited[vertex]) {
				continue;
			}

			if (minEdge[vertex] == UINT64_MAX || minEdge[vertex] > edge) {
				s.erase(std::pair<int, int>(minEdge[vertex], vertex));

				parent[vertex] = cur_vertex;
				minEdge[vertex] = edge;

				s.emplace(edge, vertex);
			}
		}
	}
}

int getMinSpanningTreeWeight(const IGraph& graph) {

	std::vector<bool> visited(graph.VerticesCount(), false);

	std::vector<int> parent(graph.VerticesCount(), -1);

	std::vector<size_t> minEdge(graph.VerticesCount(), UINT64_MAX);

	// По условию граф связный
	MinSpanningTree(graph, visited, parent, minEdge);

	return std::accumulate(minEdge.begin(), minEdge.end(), 0ll);
}

int main() {

	int n{}, m{};

	std::cin >> n >> m;

	// вершины = [1, n]
	ListGraph graph(n);

	for (int i = 0; i < m; ++i) {
		int b{}, e{}, w{};

		std::cin >> b >> e >> w;

		// чтобы удобнее работать, вершины запишу [0, n)
		graph.AddEdge(b - 1, e - 1, w);
	}

	std::cout << getMinSpanningTreeWeight(graph);

	return 0;
}
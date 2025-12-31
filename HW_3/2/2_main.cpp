#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <cstdint>

class IGraph {
public:

	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Реализация Графа

class ListGraph : public IGraph {
public:

	ListGraph(size_t size);

	void AddEdge(int from, int to) override;

	int VerticesCount() const override;

	std::vector<int> GetNextVertices(int vertex) const override;

	~ListGraph();

private:

	std::vector<std::vector<int>> adjacencyList;
};

ListGraph::ListGraph(size_t size) : adjacencyList(size) {
}

void ListGraph::AddEdge(int from, int to) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	adjacencyList[from].push_back(to);

	if (from != to) {
		adjacencyList[to].push_back(from);
	}
}

int ListGraph::VerticesCount() const {

	return static_cast<int>(adjacencyList.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	return adjacencyList[vertex];
}

ListGraph::~ListGraph() {
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// функции количества кратчайших путей

void internalShortestPathsCount(const IGraph& graph, int u, std::vector<size_t>& minPath, std::vector<size_t>& minPathCount) {
	std::queue<int> q;

	q.push(u);

	while (!q.empty()) {
		int curent = q.front();
		q.pop();

		std::vector<int> nextVertices = graph.GetNextVertices(curent);

		for (const auto& nextVertice: nextVertices) {
			if (minPath[nextVertice] > minPath[curent] + 1) {

				minPath[nextVertice] = minPath[curent] + 1;
				minPathCount[nextVertice] = minPathCount[curent];

				q.push(nextVertice);
			}
			else if (minPath[nextVertice] == minPath[curent] + 1) {
				minPathCount[nextVertice] += minPathCount[curent];
			}
		}
	}
}

size_t shortestPathsCount(const IGraph& graph, int u, int w) {

	std::vector<size_t> minPathCount(graph.VerticesCount(), 0);
	std::vector<size_t> minPath(graph.VerticesCount(), UINT64_MAX);

	minPath[u] = 0;
	minPathCount[u] = 1;

	//////////////////////////////////////////////////

	internalShortestPathsCount(graph, u, minPath, minPathCount);

	//////////////////////////////////////////////////

	return minPathCount[w];
}

int main() {

	int v{}, n{};

	std::cin >> v >> n;

	ListGraph graph(v);

	for (int i = 0; i < n; ++i) {
		int from{}, to{};

		std::cin >> from >> to;

		graph.AddEdge(from, to);
	}

	int u{}, w{};

	std::cin >> u >> w;

	std::cout << shortestPathsCount(graph, u, w);

	return 0;
}
#include "ListGraph.h"
#include <exception>

ListGraph::ListGraph(size_t size) : adjacencyList(size) {
}

ListGraph::ListGraph(const IGraph& graph) : adjacencyList(graph.VerticesCount()) {

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		adjacencyList[i] = graph.GetNextVertices(i);
	}
}

void ListGraph::AddEdge(int from, int to) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	adjacencyList[from].push_back(to);
}

int ListGraph::VerticesCount() const {

	return static_cast<int>(adjacencyList.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	return adjacencyList[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	std::vector<int> prevVertices;
	prevVertices.reserve(adjacencyList.size());

	for (int i = 0; i < adjacencyList.size(); ++i) {
		for (int j = 0; j < adjacencyList[i].size(); ++j) {
			if (adjacencyList[i][j] == vertex) {
				prevVertices.push_back(i);
				break;
			}
		}
	}

	return prevVertices;
}

ListGraph::~ListGraph() {
}
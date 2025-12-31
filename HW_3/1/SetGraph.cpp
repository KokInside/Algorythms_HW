#include "SetGraph.h"

SetGraph::SetGraph(size_t size) : adjacencySet(size) {
}

SetGraph::SetGraph(const IGraph& graph) : adjacencySet(graph.VerticesCount()) {

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> nextVertices = graph.GetNextVertices(i);

		for (int j = 0; j < nextVertices.size(); ++j) {
			adjacencySet[i].insert(nextVertices[j]);
		}
	}
}

void SetGraph::AddEdge(int from, int to) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	adjacencySet[from].insert(to);
}

int SetGraph::VerticesCount() const {

	return static_cast<int>(adjacencySet.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	std::vector<int> nextVertices;
	nextVertices.reserve(adjacencySet[vertex].size());

	for (auto& index : adjacencySet[vertex]) {
		nextVertices.push_back(index);
	}

	return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	std::vector<int> res;
	res.reserve(adjacencySet.size());

	for (int i = 0; i < adjacencySet.size(); ++i) {
		for (auto ver = adjacencySet[i].cbegin(); ver != adjacencySet[i].cend(); ver++) {
			if (*ver == vertex) {
				res.push_back(i);
				break;
			}
		}
	}

	return res;
}

SetGraph::~SetGraph() {
}
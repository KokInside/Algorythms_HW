#include "ArcGraph.h"
#include <iostream>

ArcGraph::ArcGraph(size_t size) : vertexCount(size) {
}

ArcGraph::ArcGraph(const IGraph& graph) : vertexCount(graph.VerticesCount()) {

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> nextVertices = graph.GetNextVertices(i);

		for (int j = 0; j < nextVertices.size(); ++j) {
			adjacencyArc.push_back(std::pair<int, int>(i, nextVertices[j]));
		}
	}
}

void ArcGraph::AddEdge(int from, int to) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	adjacencyArc.push_back(std::move(std::pair<int, int>(from, to)));
}

int ArcGraph::VerticesCount() const {

	return static_cast<int>(vertexCount);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	std::vector<int> nextVertices;
	nextVertices.reserve(vertexCount);

	for (int i = 0; i < adjacencyArc.size(); ++i) {
		if (adjacencyArc[i].first == vertex) {
			nextVertices.push_back(adjacencyArc[i].second);
		}
	}

	return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	std::vector<int> res;
	res.reserve(vertexCount);

	for (int i = 0; i < adjacencyArc.size(); ++i) {
		if (adjacencyArc[i].second == vertex) {
			res.push_back(adjacencyArc[i].first);
		}
	}

	return res;
}

ArcGraph::~ArcGraph() {
}
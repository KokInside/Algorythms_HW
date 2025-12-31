#include "MatrixGraph.h"
#include <iostream>

MatrixGraph::MatrixGraph(size_t size) : adjacencyMatrix(size, std::vector<int>(size, 0)) {
}

MatrixGraph::MatrixGraph(const IGraph& graph) : adjacencyMatrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0)) {

	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> vertexes = graph.GetNextVertices(i);

		for (int j = 0; j < vertexes.size(); ++j) {
			++adjacencyMatrix[i][vertexes[j]];
		}
	}
}

void MatrixGraph::AddEdge(int from, int to) {

	if (from < 0 || to < 0 || from >= VerticesCount() || to >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	++adjacencyMatrix[from][to];
}

int MatrixGraph::VerticesCount() const {

	return static_cast<int>(adjacencyMatrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	std::vector<int> nextVertices;
	nextVertices.reserve(adjacencyMatrix.size());

	for (int i = 0; i < adjacencyMatrix.size(); ++i) {
		if (adjacencyMatrix[vertex][i] != 0) {
			nextVertices.push_back(i);
		}
	}

	return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::out_of_range("Out of vertices range");
	}

	std::vector<int> prevVertices;
	prevVertices.reserve(adjacencyMatrix.size());

	for (int i = 0; i < adjacencyMatrix.size(); ++i) {
		if (adjacencyMatrix[i][vertex] != 0) {
			prevVertices.push_back(i);
		}
	}

	return prevVertices;
}

MatrixGraph::~MatrixGraph() {
}
#pragma once
#include "IGraph.h"

class MatrixGraph : public IGraph {
public:

	MatrixGraph(size_t size);

	MatrixGraph(const IGraph& graph);

	void AddEdge(int from, int to);

	int VerticesCount() const override;

	virtual std::vector<int> GetNextVertices(int vertex) const override;

	virtual std::vector<int> GetPrevVertices(int vertex) const override;

	~MatrixGraph();

private:

	std::vector<std::vector<int>> adjacencyMatrix;
};
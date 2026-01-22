#pragma once
#include "IGraph.h"

class ListGraph : public IGraph {
public:

	ListGraph(size_t size);

	ListGraph(const IGraph& graph);

	void AddEdge(int from, int to) override;

	int VerticesCount() const override;

	std::vector<int> GetNextVertices(int vertex) const override;

	std::vector<int> GetPrevVertices(int vertex) const override;

	~ListGraph();

private:

	std::vector<std::vector<int>> adjacencyList;
};
#pragma once
#include <vector>
#include <functional>
#include <queue>

class IGraph {
public:

	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

	void BFS(int vertex, const std::function<void(int)> func) const;

	void DFS(int vertex, const std::function<void(int)> func) const;

	std::deque<int> TopologicalSort();

private:

	void internalBFS(int vertex, std::vector<bool>& visited, const std::function<void(int)>& func) const;

	void internalDFS(int vertex, std::vector<bool>& visited, const std::function<void(int)>& func) const;

	void internalTopologicalSort(std::queue<int>& q, std::vector<int>& inDegree, std::deque<int>& deq) const;
};
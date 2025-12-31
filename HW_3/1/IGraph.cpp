#include "IGraph.h"

void IGraph::BFS(int vertex, const std::function<void(int)> func) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	std::vector<bool> visited(VerticesCount(), false);

	internalBFS(vertex, visited, func);

	for (int i = 0; i < VerticesCount(); ++i) {

		if (!visited[i]) {
			if (GetPrevVertices(i).size() == 0) {
				internalBFS(i, visited, func);
			}
		}
	}
}

void IGraph::DFS(int vertex, const std::function<void(int)> func) const {

	if (vertex < 0 || vertex >= VerticesCount()) {
		throw std::exception("Out of vertices range");
	}

	std::vector<bool> visited(VerticesCount(), false);

	internalDFS(vertex, visited, func);

	for (int i = 0; i < VerticesCount(); ++i) {

		if (!visited[i]) {
			if (GetPrevVertices(i).size() == 0) {
				internalDFS(i, visited, func);
			}
		}
	}
}

std::deque<int> IGraph::TopologicalSort() {

	std::deque<int> deq;

	std::vector<int> inDegree(VerticesCount(), 0);
	std::queue<int> q;

	for (int i = 0; i < VerticesCount(); ++i) {
		inDegree[i] = GetPrevVertices(i).size();
		if (inDegree[i] == 0) {
			q.push(i);
		}
	}

	internalTopologicalSort(q, inDegree, deq);

	return deq;
}


void IGraph::internalBFS(int vertex, std::vector<bool>& visited, const std::function<void(int)>& func) const {

	if (visited[vertex]) {
		return;
	}

	visited[vertex] = true;

	std::queue<int> q;
	q.push(vertex);

	while (!q.empty()) {

		int front = q.front();
		q.pop();

		func(front);

		std::vector<int> nextVertices = GetNextVertices(front);

		for (const auto& next : nextVertices) {
			if (!visited[next]) {
				visited[next] = true;

				q.push(next);
			}
		}
	}
}

void IGraph::internalDFS(int vertex, std::vector<bool>& visited, const std::function<void(int)>& func) const {

	if (visited[vertex]) {
		return;
	}

	visited[vertex] = true;

	std::stack<int> s;

	s.push(vertex);

	while (!s.empty()) {
		int top = s.top();
		s.pop();

		func(top);

		std::vector<int> nextVertices = GetNextVertices(top);

		for (const auto& next : nextVertices) {
			if (!visited[next]) {
				visited[next] = true;

				s.push(next);
			}
		}
	}
}

void IGraph::internalTopologicalSort(std::queue<int>& q, std::vector<int>& inDegree, std::deque<int>& deq) const {

	while (!q.empty()) {
		int front = q.front();
		q.pop();

		deq.push_back(front);

		std::vector<int> nextVertices = GetNextVertices(front);

		for (const auto& next : nextVertices) {
			--inDegree[next];

			if (inDegree[next] == 0) {
				q.push(next);
			}
		}
	}
}
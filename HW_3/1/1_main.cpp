#include <iostream>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

/*
* 
Необходимо написать несколько реализаций интерфейса:
- ListGraph, хранящий граф в виде массива списков смежности,
- MatrixGraph, хранящий граф в виде матрицы смежности,
- SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
- ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&.
Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.

*/

int main() {

	ListGraph startGraph(13);

	// Инициализация вершин
	startGraph.AddEdge(0, 6);
	startGraph.AddEdge(2, 1);
	startGraph.AddEdge(3, 6);
	startGraph.AddEdge(3, 10);
	startGraph.AddEdge(4, 2);
	startGraph.AddEdge(4, 5);
	startGraph.AddEdge(4, 11);
	startGraph.AddEdge(4, 12);
	startGraph.AddEdge(5, 0);
	startGraph.AddEdge(5, 1);
	startGraph.AddEdge(7, 8);
	startGraph.AddEdge(7, 10);
	startGraph.AddEdge(9, 0);
	startGraph.AddEdge(9, 3);
	startGraph.AddEdge(10, 6);
	startGraph.AddEdge(11, 12);
	startGraph.AddEdge(12, 6);

	// каждый граф копирует предыдущий
	MatrixGraph matrixGraph(startGraph);
	SetGraph setGraph(matrixGraph);
	ArcGraph arcGraph(setGraph);
	ListGraph listGraph(arcGraph);


	// BFS

	std::cout << "BFS:" << std::endl << std::endl;

	std::cout << "List Graph" << std::endl;
	listGraph.BFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Matrix Graph" << std::endl;
	matrixGraph.BFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Set Graph" << std::endl;
	setGraph.BFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Arc Graph" << std::endl;
	arcGraph.BFS(4, [](const int& a) {std::cout << a << " "; });

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


	// DFS

	std::cout << "DFS:" << std::endl << std::endl;

	std::cout << "List Graph" << std::endl;
	listGraph.DFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Matrix Graph" << std::endl;
	matrixGraph.DFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Set Graph" << std::endl;
	setGraph.DFS(4, [](const int& a) {std::cout << a << " "; });
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Arc Graph" << std::endl;
	arcGraph.DFS(4, [](const int& a) {std::cout << a << " "; });

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


	// Topological Sort

	std::cout << "Topological Sort:" << std::endl << std::endl;

	std::cout << "List Graph" << std::endl;
	for (auto& i : listGraph.TopologicalSort()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Matrix Graph" << std::endl;
	for (auto& i : matrixGraph.TopologicalSort()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Set Graph" << std::endl;
	for (auto& i : setGraph.TopologicalSort()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Arc Graph" << std::endl;
	for (auto& i : arcGraph.TopologicalSort()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	return 0;
}
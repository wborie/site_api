all: test graphDijkstra

test: Graph.h testGraph.cpp
	g++ -g -Wall -std=c++11 testGraph.cpp -o test

graphDijkstra: Graph.h graphDijkstra.cpp
	g++ -g -Wall -std=c++11 graphDijkstra.cpp -o graphDijkstra
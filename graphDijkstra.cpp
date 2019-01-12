#include "Graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <chrono>
#include <thread>
#include <stdlib.h>

int main(int argc, char const *argv[]) // Remember to manually 'make' when making changes b/c php script won't do it
{
	// std::vector<Node<int, int>*> nodes;
	// for(int i = 0; i < 5; i++)
	// {
	// 	nodes.push_back(new Node<int, int>(i, i + 10));
	// }
	// for(int i = 0; i < 5; i++)
	// {
	// 	std::cout << nodes[i]->getKey() << std::endl;
	// 	std::cout << nodes[i]->getValue() << std::endl;
	// }

	// for(int i = 0; i < 5; i++)
	// {
	// 	nodes[i]->setValue(i + 20);
	// 	std::cout << nodes[i]->getValue() << std::endl;
	// }
	if (argc != 4) { // Nodes, edges, startNode
		std::cout << "Bad input - incorrect number of arguments";
		return 0;
	}
	std::string nodeStr = argv[1];
	std::string currentNode = "";
	std::vector<std::string> nodes;
	for(int i = 1; i < nodeStr.length() - 1; i++) {
		char currentChar = nodeStr[i];
		if (currentChar == '[' || currentChar == ']') continue;
		else if (currentChar == ',') {
			nodes.push_back(currentNode);
			currentNode = "";
		}
		else {
			currentNode += currentChar;
		}
	}
	if (currentNode != "") nodes.push_back(currentNode);

	Graph<std::string> graph(nodes.size());
	for(int i = 0; i < nodes.size(); i++) {
		graph.insertNode(nodes[i]);
	}


	// Read edges
	std::string edgeStr = argv[2];
	std::string currentEdgeStart = "";
	std::string currentEdgeEnd = "";
	std::string currentEdgeWeight = "";
	bool readingStart = true;
	bool readingEnd = false;
	std::vector<std::string> edgeStarts;
	std::vector<std::string> edgeEnds;
	std::vector<std::string> edgeWeights;
	for(int i = 1; i < edgeStr.length() - 1; i++) {
		char currentChar = edgeStr[i];
		if (currentChar == '[' || currentChar == ']') continue;
		else if (currentChar == ',') {
			edgeStarts.push_back(currentEdgeStart);
			edgeEnds.push_back(currentEdgeEnd);
			edgeWeights.push_back(currentEdgeWeight);
			currentEdgeStart = "";
			currentEdgeEnd = "";
			currentEdgeWeight = "";
			readingStart = true;
		}
		else if (currentChar == '-') {
			readingStart = false;
			readingEnd = true;
		}
		else if (currentChar == '|') {
			readingEnd = false;
		}
		else {
			if (readingStart) currentEdgeStart += currentChar;
			else if (readingEnd) currentEdgeEnd += currentChar;
			else currentEdgeWeight += currentChar;
		}
	}
	if (currentEdgeStart != "") {
		edgeStarts.push_back(currentEdgeStart);
		edgeEnds.push_back(currentEdgeEnd);
		edgeWeights.push_back(currentEdgeWeight);
	}

	for(int i = 0; i < edgeStarts.size(); i++) {
		graph.addEdge(edgeStarts[i], edgeEnds[i], std::stod(edgeWeights[i]));
		graph.addEdge(edgeEnds[i], edgeStarts[i], std::stod(edgeWeights[i])); // !!!!! Must do in both directions since direction isn't specified in this implementation !!!!!
	}

	// std::cout << graph.getGraphString();
	// std::cout << "=====================" << std::endl;
	std::string startNode = argv[3];
	startNode.erase(startNode[0]);
	startNode.erase(startNode[startNode.length() - 1]);
	std::vector<std::string> dijkstraResult = graph.dijkstra(startNode);
	for(unsigned int i = 0; i < dijkstraResult.size(); i++) {
		std::cout << dijkstraResult[i] << std::endl;
	}

	return 0;
}

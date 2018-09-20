#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <cfloat>
#include <queue>
#include <functional>
#include <iostream> // !!!!! Debugging only

template <typename Node>
class Graph {	
public:
	Graph(int inputCapacity);
	~Graph();
	void deleteNodeList();
	void insertNode(Node newNode);
	void increaseCapacity();
	std::string getNodeList();
	std::string getGraphString();
	void addEdge(Node firstNode, Node secondNode, double weight);
	void deleteEdges();

	// void insertNode(Key, Value);
	// void removeNode(Key);
	// void addEdge(Key, Key, double); // May want to override this to support custom edge weight type (e.g. string weights?)
	// void removeEdge(Key, Key);
	// double getEdge(Key firstNodeKey, Key secondNodeKey);
	// std::vector<std::pair<Node*, double>> getOutgoingEdges(Key nodeKey);
	// std::vector<std::pair<Node*, double>> getIncomingEdges(Key nodeKey);
	// std::string getEdgeTable();

	// Key* getFilledNodeList();
	// void deleteFilledNodeList(Key* filledNodeList);
	// int dijkstra(Key startNode);

private:
	struct NodeContainer;
	struct Edge {
		NodeContainer* from;
		NodeContainer* to;
		double weight;
	};
	struct NodeContainer {
		Node node;
		int nodeListIndex;
		bool set;
		std::vector<Edge*> edges;
	};
	
	int capacity;
	int size;
	int currentNodeListIndex;
	int filledSize;
	NodeContainer** nodeList;

	std::vector<Edge*> getOutgoingEdges(Node node);
	// struct DijkstraNodeComparator
	// {
	// 	bool operator()(const DijkstraNode* lhs, const DijkstraNode* rhs)
	// 	{
	// 		return lhs->distance > rhs->distance;
	// 	}
	// };
};

template <typename Node>
Graph<Node>::Graph(int inputCapacity) {
	size = 0;
	filledSize = 0;
	capacity = inputCapacity;
	currentNodeListIndex = 0;
	nodeList = new NodeContainer*[capacity];
	for(int i = 0; i < capacity; i++) {
		nodeList[i] = new NodeContainer;
		nodeList[i]->set = false;
	}
}

template <typename Node>
Graph<Node>::~Graph() {
	deleteEdges();
	deleteNodeList();
}

template <typename Node>
void Graph<Node>::deleteNodeList() {
	for(int i = 0; i < capacity; i++) {
		delete nodeList[i];
	}
	delete[] nodeList;
}

template <typename Node>
void Graph<Node>::deleteEdges() {
	for(int i = 0; i < capacity; i++) {
		std::vector<Edge*> currentNodeContainerEdges = nodeList[i]->edges;
		for(unsigned int j = 0; j < currentNodeContainerEdges.size(); j++) {
			delete currentNodeContainerEdges[j];
		}
	}
}

template <typename Node>
void Graph<Node>::insertNode(Node newNode) {
	if (size == capacity) increaseCapacity();
	nodeList[currentNodeListIndex]->set = true;
	nodeList[currentNodeListIndex]->node = newNode;
	nodeList[currentNodeListIndex]->nodeListIndex = currentNodeListIndex;
	size++;
	filledSize++;
	currentNodeListIndex++;
}

template <typename Node>
void Graph<Node>::increaseCapacity() {
	int newCapacity = capacity * 2;
	NodeContainer** newNodeList = new NodeContainer*[newCapacity];
	for(int i = 0; i < newCapacity; i++) {
		newNodeList[i] = nullptr;
	}
	for(int i = 0; i < capacity; i++) {
		newNodeList[i] = nodeList[i];
	}
	for(int i = capacity; i < newCapacity; i++) {
		newNodeList[i] = new NodeContainer;
		newNodeList[i]->set = false;
	}

	delete[] nodeList;
	nodeList = newNodeList;
	capacity = newCapacity;
}

template <typename Node>
std::string Graph<Node>::getNodeList() {
	std::string result = "";
	for(int i = 0; i < capacity; i++) {
		if (nodeList[i]->set) result += nodeList[i]->node + " | ";
	}
	return result;
}

template <typename Node>
std::string Graph<Node>::getGraphString() {
	std::string result = "";
	for(int i = 0; i < capacity; i++) {
		if (nodeList[i]->set) result += nodeList[i]->node + " : ";
		std::vector<Edge*> currentNodeContainerEdges = nodeList[i]->edges;
		for(unsigned int j = 0; j < currentNodeContainerEdges.size(); j++) {
			result += "[" + currentNodeContainerEdges[j]->from->node + ", " + currentNodeContainerEdges[j]->to->node + ", " + 
			std::to_string(currentNodeContainerEdges[j]->weight) + "]";
		}
		if (nodeList[i]->set) result += "\n";
	}
	std::vector<Edge*> temp = getOutgoingEdges("first");
	for(int i = 0; i < temp.size(); i++) {
		result += temp[i]->from->node + "|" + temp[i]->to->node + "|" + std::to_string(temp[i]->weight);
	}
	return result;
}

template <typename Node>
void Graph<Node>::addEdge(Node firstNode, Node secondNode, double weight) { // first = from, second = to
	NodeContainer* firstNodeContainer = nullptr;
	NodeContainer* secondNodeContainer = nullptr;
	for(int i = 0; i < capacity; i++) {
		if (nodeList[i]->set) {
			if (nodeList[i]->node == firstNode) firstNodeContainer = nodeList[i];
			else if (nodeList[i]->node == secondNode) secondNodeContainer = nodeList[i];
		}
	}
	if (firstNodeContainer == nullptr || secondNodeContainer == nullptr) return; // To and from nodes aren't in the graph

	Edge* newEdge = new Edge;
	newEdge->from = firstNodeContainer;
	newEdge->to = secondNodeContainer;
	newEdge->weight = weight;
	firstNodeContainer->edges.push_back(newEdge);
}

template <typename Node>
std::vector<typename Graph<Node>::Edge*> Graph<Node>::getOutgoingEdges(Node currentNode) {
	std::vector<Edge*> result;
	NodeContainer* currentNodeContainer = nullptr;
	for(int i = 0; i < capacity; i++) {
		if (nodeList[i]->set) {
			if (nodeList[i]->node == currentNode) currentNodeContainer = nodeList[i];
		}
	}
	if (currentNodeContainer == nullptr) return result;
	else return currentNodeContainer->edges;
}


// template <typename Node>
// typename Graph<Node>::Edge* Graph<Node>::addEdge(Node firstNode, Node secondNode, double weight) { // first = from, second = to
// 	std::cout << "Added an edge";
// 	return new Edge;
// }


// /* Adds an edge if it doesn't already exist and overrides an edge if it does exist*/
// template <typename Key, typename Value>
// void Graph<Key, Value>::addEdge(Key firstNodeKey, Key secondNodeKey, double edgeWeight) // first = from, second = to
// {
// 	if (firstNodeKey == secondNodeKey) return; // Don't allow self loops
// 	typename std::map<Key, Node*>::iterator it1 = nodeMap.find(firstNodeKey);
// 	if (it1 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	typename std::map<Key, Node*>::iterator it2 = nodeMap.find(secondNodeKey);
// 	if (it2 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	edgeTable[it1->second->nodeListIndex][it2->second->nodeListIndex] = edgeWeight;

// }


// // Doesn't check for if a node already exists in the graph
// template <typename Key, typename Value>
// void Graph<Key, Value>::insertNode(Key insertKey, Value insertValue)
// { 
// 	if (size == capacity) increaseCapacity();
// 	NodeContainer* newNode = new Node();
// 	newNode->key = insertKey;
// 	newNode->value = insertValue;
// 	newNode->nodeListIndex = currentNodeListIndex;
// 	newNode->filledNodeListIndex = filledSize;
// 	nodeMap[insertKey] = newNode;
// 	nodeList[currentNodeListIndex] = insertKey;
// 	currentNodeListIndex++;
// 	size++;
// 	filledSize++;
// }

// template <typename Key, typename Value>
// void Graph<Key, Value>::removeNode(Key removeKey)
// { 
// 	typename std::map<Key, Node*>::iterator it = nodeMap.find(removeKey);
// 	if (it == nodeMap.end()) return; // Node with this key doesn't exist
// 	int nodeListPosToRemove = it->second->nodeListIndex;
// 	nodeList[nodeListPosToRemove] = ""; // !!!!! Compilation issues if Key is a primitive <-- now assuming Keys are strings
// 	for(int i = 0; i < capacity; i++)
// 	{
// 		edgeTable[nodeListPosToRemove][i] = 0;
// 		edgeTable[i][nodeListPosToRemove] = 0;
// 	}

// 	for(int i = it->second->filledNodeListIndex; i < capacity; i++) {
// 		if (nodeList[i] != "") {
// 			it = nodeMap.find(nodeList[i]); // !!!!! This causes O(nlogn), could be fixed by keeping a nodeList of Nodes instead of Keys
// 			it->second->filledNodeListIndex--;
// 		}
// 	}

// 	delete nodeMap[it->second];
// 	nodeMap.erase(it);
// 	filledSize--;
// }

// /* Adds an edge if it doesn't already exist and overrides an edge if it does exist*/
// template <typename Key, typename Value>
// void Graph<Key, Value>::addEdge(Key firstNodeKey, Key secondNodeKey, double edgeWeight) // first = from, second = to
// {
// 	if (firstNodeKey == secondNodeKey) return; // Don't allow self loops
// 	typename std::map<Key, Node*>::iterator it1 = nodeMap.find(firstNodeKey);
// 	if (it1 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	typename std::map<Key, Node*>::iterator it2 = nodeMap.find(secondNodeKey);
// 	if (it2 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	edgeTable[it1->second->nodeListIndex][it2->second->nodeListIndex] = edgeWeight;

// }

// template <typename Key, typename Value>
// void Graph<Key, Value>::removeEdge(Key firstNodeKey, Key secondNodeKey) // first = from, second = to
// {
// 	if (firstNodeKey == secondNodeKey) return; // Don't allow self loops
// 	typename std::map<Key, Node*>::iterator it1 = nodeMap.find(firstNodeKey);
// 	if (it1 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	typename std::map<Key, Node*>::iterator it2 = nodeMap.find(secondNodeKey);
// 	if (it2 == nodeMap.end()) return; // One of the two nodes doesn't exist
// 	edgeTable[it1->second->nodeListIndex][it2->second->nodeListIndex] = 0;
// }

// /* Returns value of edge between first and second, 0 if no edge exists */
// template <typename Key, typename Value>
// double Graph<Key, Value>::getEdge(Key firstNodeKey, Key secondNodeKey) // first = from, second = to
// {
// 	if (firstNodeKey == secondNodeKey) return 0; // Don't allow self loops
// 	typename std::map<Key, Node*>::iterator it1 = nodeMap.find(firstNodeKey);
// 	if (it1 == nodeMap.end()) return 0; // One of the two nodes doesn't exist
// 	typename std::map<Key, Node*>::iterator it2 = nodeMap.find(secondNodeKey);
// 	if (it2 == nodeMap.end()) return 0; // One of the two nodes doesn't exist
// 	return edgeTable[it1->second->nodeListIndex][it2->second->nodeListIndex];
// }

// /* Returns a vector of pairs, the first value being the key of the 'to' vertex, the second value being the edge weight */
// template <typename Key, typename Value>
// std::vector<std::pair<Node*, double>> Graph<Key, Value>::getOutgoingEdges(Key nodeKey)
// {
// 	std::vector<std::pair<Key, double>> result;
// 	typename std::map<Key, Node*>::iterator it = nodeMap.find(nodeKey);
// 	if (it == nodeMap.end()) return result; // Node doesn't exist
// 	int numOutgoingEdges = 0;
// 	int nodeListPos = it->second->nodeListIndex;
// 	for(int i = 0; i < size; i++)
// 	{
// 		if (edgeTable[nodeListPos][i] != 0) numOutgoingEdges++;
// 	}
// 	for(int i = 0; i < size; i++)
// 	{
// 		if (edgeTable[nodeListPos][i] != 0)
// 		{
// 			result.push_back(std::make_pair((nodeMap.find(nodeList[i]))->second, edgeTable[nodeListPos][i]));
// 		}
// 	}
// 	return result;
// }

// /* Returns a vector of pairs, the first value being the key of the 'from' vertex, the second value being the edge weight */
// template <typename Key, typename Value>
// std::vector<std::pair<Node*, double>> Graph<Key, Value>::getIncomingEdges(Key nodeKey)
// {
// 	std::vector<std::pair<Key, double>> result;
// 	typename std::map<Key, Node*>::iterator it = nodeMap.find(nodeKey);
// 	if (it == nodeMap.end()) return result; // Node doesn't exist
// 	int numIncomingEdges = 0;
// 	int nodeListPos = it->second->nodeListIndex;
// 	for(int i = 0; i < size; i++)
// 	{
// 		if (edgeTable[i][nodeListPos] != 0) numIncomingEdges++;
// 	}
// 	for(int i = 0; i < size; i++)
// 	{
// 		if (edgeTable[i][nodeListPos] != 0)
// 		{
// 			result.push_back(std::make_pair((nodeMap.find(nodeList[i]))->second, edgeTable[i][nodeListPos]));
// 		}
// 	}
// 	return result;
// }

// template <typename Key, typename Value>
// std::string Graph<Key, Value>::getEdgeTable()
// {
// 	std::string result;
// 	for(int row = 0; row < capacity; row++)
// 	{
// 		// result += std::to_string(row) + " |";
// 		result += nodeList[row] + " |"; // !!!!! This will cause a seg fault if Keys aren't strings b/c no to_string used
// 		// std::cout << nodeList[row] + " |";
// 		for(int col = 0; col < capacity; col++)
// 		{
// 			if (col == 0) result += " ";
// 			result += std::to_string(edgeTable[row][col]);
// 			if (col != capacity - 1) result += " | ";
// 		}
// 		result += "\n";
// 	}
// 	return result;
// }

// template <typename Key, typename Value>
// void Graph<Key, Value>::increaseCapacity()
// {
// 	int newCapacity = capacity * 2;
// 	double** newEdgeTable = new double*[newCapacity];
// 	// Create new edgeTable
// 	for(int row = 0; row < newCapacity; row++)
// 	{
// 		newEdgeTable[row] = new double[newCapacity];
// 		for(int col = 0; col < newCapacity; col++)
// 		{
// 			newEdgeTable[row][col] = 0;
// 		}
// 	}
// 	// Fill in existing values
// 	for(int row = 0; row < capacity; row++)
// 	{
// 		for(int col = 0; col < capacity; col++)
// 		{
// 			newEdgeTable[row][col] = edgeTable[row][col];
// 		}
// 	}

// 	deleteEdgeTable();
// 	edgeTable = newEdgeTable;

// 	//Copy nodeList to new array of new capacity
// 	Key* newNodeList = new Key[newCapacity];
// 	for(int i = 0; i < capacity; i++)
// 	{
// 		newNodeList[i] = nodeList[i];
// 	}
// 	delete[] nodeList;
// 	nodeList = newNodeList;
	
// 	capacity = newCapacity;
// }

// template <typename Key, typename Value>
// void Graph<Key, Value>::deleteEdgeTable()
// {
// 	for(int row = 0; row < capacity; row++)
// 	{
// 		delete[] edgeTable[row];
// 	}
// 	delete[] edgeTable;
// }

// // Should always be accompanied by a call to deleteFilledNodeList
// template <typename Key, typename Value> 
// Key* Graph<Key, Value>::getFilledNodeList() {
// 	Key* filledNodeList = new Key[filledSize];
// 	int filledIndex = 0;
// 	for(int i = 0; i < size; i++) {
// 		if (nodeList[i] != "") {
// 			filledNodeList[filledIndex] = nodeList[i];
// 			filledIndex++;
// 		}
// 	}
// 	return filledNodeList;
// }

// template <typename Key, typename Value>
// void Graph<Key, Value>::deleteFilledNodeList(Key* filledNodeList) {
// 	delete[] filledNodeList;
// }

// template <typename Key, typename Value>
// int Graph<Key, Value>::dijkstra(Key startNode) { //!!!!! remember to change declaration from void !!!!!
// 	typename std::map<Key, Node*>::iterator it = nodeMap.find(startNode);
// 	if (it == nodeMap.end()) return 0; // The node doesn't exist

// 	Key* filledNodeList = getFilledNodeList();
// 	Key* parent = new Key[filledSize];
// 	double* distance = new double[filledSize];
// 	bool* inQueue = new bool[filledSize];
// 	std::priority_queue<DijkstraNode*, std::vector<DijkstraNode*>, DijkstraNodeComparator> queue;
// 	for(int i = 0; i < filledSize; i++) {
// 		parent[i] = "";
// 		distance[i] = DBL_MAX;
// 		if (filledNodeList[i] == startNode) distance[i] = 0;
// 		DijkstraNode* node = new DijkstraNode(); // !!!!! remember to free these
// 		node->key = filledNodeList[i];
// 		node->distance = distance[i];
// 		node->index = i;
// 		queue.push(node);
// 		inQueue[i] = true;
// 	}

// 	while (!queue.empty()) {
// 		DijkstraNode* currentNode = queue.top();
// 		queue.pop();
// 		int currentIndex = currentNode->index;
// 		inQueue[currentIndex] = false;
// 		std::vector<std::pair<Key, double>> adjacentNodes = getOutgoingEdges(currentNode->key);
// 		for(int i = 0; i < adjacentNodes.size(); i++) {
// 			double choice = distance[currentIndex] + adjacentNodes[i].second;
// 			std::cout << nodeMap.find(adjacentNodes[i].first);
// 			if (choice < distance[nodeMap.find(adjacentNodes[i].first)->filledNodeListIndex]) { // !!!!! just eat the performance hit for now
// 				continue;
// 			}
// 		}


// 		// use currentNode->index to change parent[u] distance[u]
// 		// std::cout << currentNode->key;
// 		// std::cout << adjacentNodes[0].first;
// 		// std::cout << adjacentNodes[0].second;
// 	}


// 	// std::vector<std::pair<Value, double>> Graph<Key, Value>::getOutgoingEdges(Key nodeKey)

// 	// std::cout << "Printing initial parent and distance arrays..." << std::endl;
// 	// for(int i = 0; i < filledSize; i++) {
// 	// 	std::cout << filledNodeList[i] << std::endl;
// 	// 	std::cout << parent[i] << std::endl;
// 	// }
// 	// for(int i = 0; i < filledSize; i++) {
// 	// 	std::cout << filledNodeList[i] << std::endl;
// 	// 	std::cout << distance[i] << std::endl;
// 	// }
// 	// std::cout << "Done printing initial parent and distance arrays." << std::endl;
// 	deleteFilledNodeList(filledNodeList);
// 	delete[] parent;
// 	delete[] distance;
// 	delete[] inQueue;
// }


// double** newEdgeTable = new double*[capacity];
// 	for(int row = 0; row < capacity; row++)
// 	{
// 		newEdgeTable[row] = new double[capacity];

#endif
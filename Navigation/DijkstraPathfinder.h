#pragma once

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Node;
class Connection;

class DijkstraPathfinder :public GridPathfinder
{
public:
	// Data structure to hold any necessary data for Dijkstra algorithm
	struct NodeRecord
	{
		Node* node;
		Connection* connection;
		float costSoFar;

		NodeRecord()
		{
			node = nullptr; connection = nullptr; costSoFar = 0;
		}

		NodeRecord(Node* n, Connection* c, const float& cost)
		{
			node = n;
			connection = c;
			costSoFar = cost;
		}
	};

	DijkstraPathfinder(Graph* pGraph);
	~DijkstraPathfinder();

	NodeRecord getSmallestInVector(std::vector<NodeRecord>& vec);
	bool containsNode(std::vector<NodeRecord>& vec, Node* node);
	NodeRecord findRecord(std::vector<NodeRecord>& vec, Node* node);
	void eraseRecord(std::vector<NodeRecord>& vec, NodeRecord rec);
	Path reversePath(Path& path);

	Path findPath(Node* pFrom, Node* pTo);

private:

};


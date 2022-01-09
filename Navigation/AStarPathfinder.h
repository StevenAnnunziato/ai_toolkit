#pragma once

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Node;
class Connection;

class AStarPathfinder :public GridPathfinder
{
public:
	struct NodeRecord
	{
		Node* node;
		Connection* connection;
		float costSoFar;
		float estimatedTotalCost;

		NodeRecord()
		{
			node = nullptr; connection = nullptr; costSoFar = 0; estimatedTotalCost = 0;
		}

		NodeRecord(Node* n, Connection* c, const float& cost, const float& estimate)
		{
			node = n;
			connection = c;
			costSoFar = cost;
			estimatedTotalCost = estimate;
		}
	};

	AStarPathfinder(Graph* pGraph);
	~AStarPathfinder();

	NodeRecord getSmallestInVector(std::vector<NodeRecord>& vec);
	bool containsNode(std::vector<NodeRecord>& vec, Node* node);
	NodeRecord findRecord(std::vector<NodeRecord>& vec, Node* node);
	void eraseRecord(std::vector<NodeRecord>& vec, NodeRecord rec);
	Path reversePath(Path& path);
	float estimateHueristic(Node* fromNode, Node* goalNode);

	Path findPath(Node* pFrom, Node* pTo);

private:

};


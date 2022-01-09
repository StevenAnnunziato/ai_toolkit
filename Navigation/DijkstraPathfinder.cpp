#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <algorithm>

using namespace std;
typedef DijkstraPathfinder::NodeRecord NodeRecord;

DijkstraPathfinder::DijkstraPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
}

DijkstraPathfinder::~DijkstraPathfinder()
{
}

// NOTE:
// This could be optimized by using a priority queue instead of a vector,
// which would remove the need for this function.
NodeRecord DijkstraPathfinder::getSmallestInVector(vector<NodeRecord>& vec)
{
	// init the smallest element
	NodeRecord smallest = vec[0];

	for (UINT i = 1; i < vec.size(); i++)
	{
		if (vec[i].costSoFar < smallest.costSoFar)
			smallest = vec[i];
	}

	return smallest;
}

bool DijkstraPathfinder::containsNode(std::vector<NodeRecord>& vec, Node* node)
{
	// linear search for the node
	for (UINT i = 0; i < vec.size(); i++)
	{
		if (vec[i].node == node)
			return true;
	}

	// search failed to find the node
	return false;
}

NodeRecord DijkstraPathfinder::findRecord(std::vector<NodeRecord>& vec, Node* node)
{
	// linear search for the record
	for (UINT i = 0; i < vec.size(); i++)
	{
		if (vec[i].node == node)
			return vec[i];
	}

	// return dummy data - this is bad practice but I can get away with it in this context
	return vec[0];
}

void DijkstraPathfinder::eraseRecord(vector<NodeRecord>& vec, NodeRecord rec)
{
	// linear search for the record
	for (UINT i = 0; i < vec.size(); i++)
	{
		if (vec[i].node == rec.node && vec[i].connection == rec.connection && vec[i].costSoFar == rec.costSoFar)
		{
			vec.erase(vec.begin() + i);
			return;
		}
	}
}

Path DijkstraPathfinder::reversePath(Path& path)
{
	Path newPath;
	for (int i = path.getNumNodes() - 1; i >= 0; i--)
	{
		newPath.addNode(path.peekNode(i));
	}
	return newPath;
}

Path DijkstraPathfinder::findPath(Node* pFrom, Node* pTo)
{
	// set up performance timer
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	// set up path visualization
#ifdef VISUALIZE_PATH
	mPath.clear();
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	Path path;
	
	// Initialize a record for the start node
	NodeRecord startRecord(pFrom, nullptr, 0);

	// Create the open and closed lists along with some other data
	vector<NodeRecord> openList;
	vector<NodeRecord> closedList;
	NodeRecord current;
	openList.push_back(startRecord);

	// Process each node
	while (openList.size() > 0)
	{
		// get the smallest element from the open list
		current = getSmallestInVector(openList);

		// if it's the goal, terminate
		if (current.node == pTo)
			break;

		// otherwise, get all its outgoing connections
		vector<Connection*> connections = mpGraph->getConnections(*current.node);

		// Loop through each connection from this node
		for (UINT i = 0; i < connections.size(); i++)
		{
			// Get the cost estimate for the next node traversal
			Node* pNextNode = connections[i]->getToNode();
			float nextNodeCost = current.costSoFar + connections[i]->getCost();

			// skip if the node is closed
			if (containsNode(closedList, pNextNode))
				continue;

			// skip if the node is open yet produces a worse route
			else if (containsNode(openList, pNextNode))
			{
				// find the open list record corresponding to this node
				NodeRecord nextNodeRecord = findRecord(openList, pNextNode);
				if (nextNodeRecord.costSoFar <= nextNodeCost)
					continue;
			}

			// otherwise this is an unvisited node so make a record for it
			NodeRecord nextNodeRecord(pNextNode, connections[i], nextNodeCost);

			// add it to the open list
			openList.push_back(nextNodeRecord);

			// visualize
#ifdef VISUALIZE_PATH
			mVisitedNodes.push_back(pNextNode);
#endif
		}

		// We are now done looking at the current node's connections,
		// so add it to the closed list and remove it from the open list.
		eraseRecord(openList, current);
		closedList.push_back(current);
	}

	// We now either found the goal or there are no more nodes to search
	if (current.node != pTo)
	{
		// no more nodes, so there is no path to the goal
		// return NULL;
	}
	else
	{
		// found the goal, so walk the path backwards adding nodes
		path.addNode(current.node);
		while (current.node != pFrom)
		{
			path.addNode(current.connection->getFromNode());
			current = findRecord(closedList, current.connection->getFromNode());
		}
	}

	// reverse path
	path = reversePath(path);

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mPath = path;
#endif
	return path;
}


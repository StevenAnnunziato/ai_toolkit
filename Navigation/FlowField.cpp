#include <list>

#include "FlowField.h"
#include "Grid.h"
#include "Game.h"

FlowField::FlowField(int numRows, int numColumns)
{
	mNumRows = numRows;
	mNumColumns = numColumns;

	mpHeatmap = new MultiDimensionalArray<int>(numRows, numColumns, INT_MAX);
	mpVectorField = new MultiDimensionalArray<Vector2D>(numRows, numColumns, Vector2D());
}

FlowField::~FlowField()
{
	delete mpHeatmap;
	mpHeatmap = nullptr;

	delete mpVectorField;
	mpVectorField = nullptr;
}

// This is just a breadth-first search noting the number of steps it took to get to each node.
// This is also called a wavefront or brushfire algorithm.
void FlowField::generateHeatmap(int goalCellIndex)
{
	// reset all values of the field to a large number
	delete mpHeatmap;
	mpHeatmap = new MultiDimensionalArray<int>(mNumRows, mNumColumns, INT_MAX);

	// set the goal to zero
	mpHeatmap->set(goalCellIndex, 0);

	// initialize the open list
	std::list<int> openList;
	openList.push_back(goalCellIndex);

	while (openList.size() > 0)
	{
		int currentIndex = openList.front();
		openList.pop_front();

		// get all neighbors of this cell
		std::vector<int> neighbors = gpGame->getGrid()->getDirectAdjacentIndices(currentIndex);

		// iterate through all of the neighbors
		for (int i = 0; i < (int)neighbors.size(); i++)
		{
			// if this node is marked or a wall, skip
			if (*mpHeatmap->get(neighbors[i]) != INT_MAX || gpGame->getGrid()->getValueAtIndex(neighbors[i]) == 1)
				continue;

			// assign this neighbor's value to the previous +1
			int neighborCost = *mpHeatmap->get(currentIndex) + 1;
			mpHeatmap->set(neighbors[i], neighborCost);

			openList.push_back(neighbors[i]);
		}
	}

	// Now that the heatmap is generated, calculate the actual flow field.
	generateVectorField();
}

void FlowField::generateVectorField()
{

	// iterate through the flow field
	unsigned int currentIndex = 0;
	do {

		// get neighbors in all 8 directions
		std::vector<int> neighbors = gpGame->getGrid()->getAdjacentIndices(currentIndex);

		// find neighbor with smallest heatmap value
		int smallest = neighbors[0];
		for (int i = 1; i < (int)neighbors.size(); i++)
		{
			if (*mpHeatmap->get(neighbors[i]) < *mpHeatmap->get(smallest))
				smallest = neighbors[i];
		}

		// populate this cell of the vector field with a vector from here to the shortest neighbor
		Vector2D vecToLowest(gpGame->getGrid()->getCellCoordsFromIndex(smallest) - gpGame->getGrid()->getCellCoordsFromIndex(currentIndex));
		mpVectorField->set(currentIndex, vecToLowest.getNormalizedVector()); // normalize the vector to avoid steering problems

		currentIndex++;

	}
	while (mpVectorField->get(currentIndex) != NULL);

}
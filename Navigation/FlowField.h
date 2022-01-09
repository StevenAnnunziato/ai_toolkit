#include "Trackable.h"
#include "MultiDimensionalArray.h"
#include "Vector2D.h"

class FlowField : public Trackable
{
public:
	FlowField(int numRows, int numColumns);
	~FlowField();

	MultiDimensionalArray<int>* getHeatmap() { return mpHeatmap; }
	MultiDimensionalArray<Vector2D>* getVectorField() { return mpVectorField; }

	void generateHeatmap(int goalCellIndex);

private:
	MultiDimensionalArray<int>* mpHeatmap;
	MultiDimensionalArray<Vector2D>* mpVectorField;
	int mNumRows;
	int mNumColumns;

	void generateVectorField();

};
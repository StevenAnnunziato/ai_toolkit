#include <Trackable.h>
#include "Steering.h"

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID);
	Vector2D weightedBlend(std::vector<Vector2D> inputVectors, std::vector<float> weights);

protected:
	virtual Steering* getSteering();
};
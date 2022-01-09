#include <Trackable.h>
#include "Steering.h"

class AlignmentSteering : public Steering
{
public:
	AlignmentSteering(const UnitID& ownerID);

protected:
	virtual Steering* getSteering();
};
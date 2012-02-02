#include "ForwardForceEvent.h"

ForwardForceEvent::ForwardForceEvent(btScalar force, btScalar forceNorm)
{
	force = forceIn; 
	forceNorm = forceNormIn;
}

btScalar ForwardForceEvent::getForce(){return forceIn;}

btScalar ForwardForceEvent::getNormForce(){return forceNormIn;}

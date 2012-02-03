#include "ForwardForceEvent.h"

ForwardForceEvent::ForwardForceEvent(btScalar force, btScalar forceNorm)
{
	force = forceIn; 
	forceNorm = forceNormIn;
}

btScalar ForwardForceEvent::getForce(){return force;}

btScalar ForwardForceEvent::getNormForce(){return forceNorm;}

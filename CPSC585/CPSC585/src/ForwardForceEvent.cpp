#include "ForwardForceEvent.h"

ForwardForceEvent::ForwardForceEvent(btScalar forceIn, btScalar forceNormIn)
{
	force = forceIn; 
	forceNorm = forceNormIn;
}

btScalar ForwardForceEvent::getForce(){return force;}

btScalar ForwardForceEvent::getNormForce(){return forceNorm;}

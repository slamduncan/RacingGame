#ifndef METHODOBSERVER_H
#define METHODOBSERVER_H

#include "TypedObserver.h"
template<ET, ObservingClassType>
class MethodObserver : public TypedObserver{
	

	virtual void (ObservingClassType::*method)(ET *e);
	ObservingClassType *objectInstance;

	void Observe(ET *e){
		(objectInstance->*method)(e);
	}
};
#endif
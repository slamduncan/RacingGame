#ifndef METHODOBSERVER_H
#define METHODOBSERVER_H

#include "TypedObserver.h"

template <class ET, class ObservingClassType>
class MethodObserver : public TypedObserver<ET>{
public:
	typedef void (ObservingClassType::*Method)(ET *e);
	MethodObserver(ObservingClassType * objectInstance, Method m)	: objectInstance(objectInstance) {funcPointer = m;}	
	ObservingClassType *objectInstance;
	Method funcPointer;
	void Observe(ET *e){
		//(objectInstance->*Method)(e);
		(objectInstance->*funcPointer)(e);
	}
};
#endif
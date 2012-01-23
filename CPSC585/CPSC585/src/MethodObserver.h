#ifndef METHODOBSERVER_H
#define METHODOBSERVER_H

#include "TypedObserver.h"

template <class ET, class ObservingClassType>
class MethodObserver : public TypedObserver<ET>{
public:
	MethodObserver(ObservingClassType * objectInstance) : objectInstance(objectInstance){ }
	void (ObservingClassType::*Method)(ET *e);
	ObservingClassType *objectInstance;

	void Observe(ET *e){
		(objectInstance->*Method)(e);
	}
};
#endif
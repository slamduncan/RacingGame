#include "TypedObserver.h"
#include "ButtonEvent.h"

class TestClass : public TypedObserver<ButtonEvent>{

public:
	void Observe(ButtonEvent *e);

};

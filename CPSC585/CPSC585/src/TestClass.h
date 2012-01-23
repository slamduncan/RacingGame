
#include "MethodObserver.h"
#include "ButtonEvent.h"

class TestClass{ //: public MethodObserver<ButtonEvent, TestClass>{

public:
	//void Observe(ButtonEvent *e);
	MethodObserver<ButtonEvent, TestClass> mo;
	TestClass();

	void testMethod(ButtonEvent *e);


};


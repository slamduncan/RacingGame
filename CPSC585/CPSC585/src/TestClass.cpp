
#include "TestClass.h"
#include <stdio.h>

//void TestClass::Observe(ButtonEvent *e){
//	fprintf(stderr, "Found Event \n");
//}
TestClass::TestClass() : mo(this, &TestClass::testMethod)
{
}

void TestClass::testMethod(ButtonEvent *e){
	fprintf(stderr, "Found Event \n");
}

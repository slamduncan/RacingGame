#include "tinyxml.h"

class ProjectNumbers {

public:
	int* CONTROLLER_Turning;
	int test;

	ProjectNumbers(){test = 0; CONTROLLER_Turning = &test;}


	void readVariablesIn(){
		TiXmlDocument doc("./magicNumbers/Controller.xml");
		doc.LoadFile();

		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);				

		//Controller: Turning Constant
		{
			pElem = hDoc.FirstChildElement().FirstChild().Element();
			if (!pElem) return;

			pElem->QueryIntAttribute("value", &test);
		}

	}

};
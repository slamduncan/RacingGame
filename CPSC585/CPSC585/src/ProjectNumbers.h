//#define TIXML_USE_STL
#include <fstream>

#include "tinyxml.h"
#include "tinystr.h"


class ProjectNumbers {

public:
	int* CONTROLLER_Turning;
	int test;

	ProjectNumbers(){test = 0; CONTROLLER_Turning = &test;}


	void readVariablesIn(){
		
		std::ifstream infile("../CPSC585/magicNumbers/Controller.xml");

		if(infile)
		{	
			printf("file found\n");
			
			TiXmlDocument doc("../CPSC585/magicNumbers/Controller.xml");
			//TiXmlDocument doc = TiXmlDocument("../CPSC585/magicNumbers/Controler.xml");
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
		else
		{
			printf("Error: Cannot open XML file\n");
		}

	}

};
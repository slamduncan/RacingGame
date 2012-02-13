#include "ProjectNumbers.h"

void ProjectNumbers::readController(){
		std::ifstream infile("../CPSC585/magicNumbers/Controller.xml");

		if(infile)
		{	
			printf("Controller file found\n");
			
			TiXmlDocument doc("../CPSC585/magicNumbers/Controller.xml");
			//TiXmlDocument doc = TiXmlDocument("../CPSC585/magicNumbers/Controler.xml");
			doc.LoadFile();
		
			TiXmlHandle hDoc(&doc);
			TiXmlElement* pElem;
			TiXmlHandle hRoot(0);				

			//Controller: Turning Constant
			{
				pElem = hDoc.FirstChildElement().FirstChild().Element();
				if (pElem) 
					pElem->QueryFloatAttribute("value", &rotateModifier);	
			}
			
		}
		else
		{
			printf("Error: Cannot open XML file\n");
		}
}

void ProjectNumbers::readPhysics(){
		std::ifstream infile("../CPSC585/magicNumbers/Physics.xml");

		if(infile)
		{	
			printf("Physics file found\n");
			
			TiXmlDocument doc("../CPSC585/magicNumbers/Physics.xml");
			//TiXmlDocument doc = TiXmlDocument("../CPSC585/magicNumbers/Controler.xml");
			doc.LoadFile();
		
			TiXmlHandle hDoc(&doc);
			TiXmlElement* pElem;
			TiXmlHandle hRoot(0);				
			TiXmlHandle top = hDoc.FirstChildElement();

			//Phsyics: kValue
			{
				pElem = hDoc.FirstChildElement().ChildElement(0).Element();				
				if (pElem) 
					pElem->QueryFloatAttribute("value", &kModifier);	
			}
			//Phsyics: damping Value
			{
				pElem = hDoc.FirstChildElement().ChildElement(1).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &cModifier);
			}
			//Physics: Gravity Value
			{
				pElem = hDoc.FirstChildElement().ChildElement(2).Element();
				float x, y, z;
				if (pElem)
				{
					pElem->QueryFloatAttribute("xComponent", &x); 
					pElem->QueryFloatAttribute("yComponent", &y);
					pElem->QueryFloatAttribute("zComponent", &z);
					gravity = btVector3(x, y, z);
				}

			}
			
		}
		else
		{
			printf("Error: Cannot open XML file\n");
		}
}
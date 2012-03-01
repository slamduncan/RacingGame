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
					pElem->QueryFloatAttribute("value", &controllerInfo.rotateModifier);	
			}
			
		}
		else
		{
			printf("Error: Cannot open XML file\n");
		}
}


void ProjectNumbers::readAI(){
		std::ifstream infile("../CPSC585/magicNumbers/AI.xml");

		if(infile)
		{	
			printf("AI file found\n");
			
			TiXmlDocument doc("../CPSC585/magicNumbers/AI.xml");
			//TiXmlDocument doc = TiXmlDocument("../CPSC585/magicNumbers/Controler.xml");
			doc.LoadFile();
		
			TiXmlHandle hDoc(&doc);
			TiXmlElement* pElem;
			TiXmlHandle hRoot(0);				

			//AI: Turning Constant
			{
				pElem = hDoc.FirstChildElement().ChildElement(0).Element();
				if (pElem) 
					pElem->QueryFloatAttribute("value", &aiInfo.rotateModifier);	
			}
			//AI Driving Constant
			{
				pElem = hDoc.FirstChildElement().ChildElement(1).Element();
				if (pElem)
					pElem->QueryFloatAttribute("value", &aiInfo.drivingModifier);
			}
			//AI max forward force
			{
				pElem = hDoc.FirstChildElement().ChildElement(2).Element();
				if (pElem)
					pElem->QueryFloatAttribute("value", &aiInfo.maxMovementForce);
			}

			//AI nextWaypoint area.
			{
				pElem = hDoc.FirstChildElement().ChildElement(3).Element();
				if (pElem)
				{
					pElem->QueryFloatAttribute("beforeValue", &aiInfo.goToNextWaypointDistanceBefore);
					pElem->QueryFloatAttribute("afterValue", &aiInfo.goToNextWaypointDistanceAfter);
				}
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
					pElem->QueryFloatAttribute("value", &physicsInfo.kModifier);	
			}
			//Phsyics: damping Value
			{
				pElem = hDoc.FirstChildElement().ChildElement(1).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.cModifier);
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
					physicsInfo.gravity = btVector3(x, y, z);
				}

			}

			//Phyics: Friction Value
			{
				pElem = hDoc.FirstChildElement().ChildElement(3).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.staticFrictionModifier);
				
				pElem = hDoc.FirstChildElement().ChildElement(4).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.dynamicFrictionModifier);
			}
			//Physics: ForwardForceModifier
			{
				pElem = hDoc.FirstChildElement().ChildElement(5).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.forwardForceModifier);
			}
			//Physics: sideFrictionModifier
			{
				pElem = hDoc.FirstChildElement().ChildElement(6).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.sideFrictionModifier);
			}
			
			//Physics: forwardFrictionModifier
			{
				pElem = hDoc.FirstChildElement().ChildElement(7).Element();
				if(pElem)
					pElem->QueryFloatAttribute("value", &physicsInfo.forwardFrictionModifier);
			}
		}
		else
		{
			printf("Error: Cannot open XML file\n");
		}
}
#include "EventTypes.h"
#include "EventSystemHandler.h"
#include "Event.h"
#include "ObservingList.h"

//EventSystemHandler EventSystemHandler::instance;
EventSystemHandler* EventSystemHandler::instance = 0;

EventSystemHandler::EventSystemHandler(){	
	//list = std::vector<ObservingList>;	
	for(int i = 0; i < EventTypes::EVENTS_NUM; i++){
		list.push_back(ObservingList());
	}
}

void EventSystemHandler::emitEvent(Event *e){
	//Add code to emit the event (look at the type and index into array of registered observers)
	list[e->getType()].updateAll(e);
	//delete e;
}

void EventSystemHandler::addObserver(Observer *o, EventTypes::EventType type){
	list[type].add(o);	
}

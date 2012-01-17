#include "Event.h"
#include "Observer.h"
#include "ObservingList.h"
#include "EventTypes.h"
#include "TypedObserver.h"

#ifndef EVENTSYSTEMHANDLER_H
#define EVENTSYSTEMHANDLER_H

class EventSystemHandler {

private:
	static EventSystemHandler *instance;// Use this with a construct method to then allow 
										// for more control over when initialization happens
	//static EventSystemHandler instance;
	EventSystemHandler();
	EventSystemHandler (const EventSystemHandler&);
	EventSystemHandler& operator= (const EventSystemHandler&);
	std::vector<ObservingList> list;

public:
	/*
	static EventSystemHandler &getInstance(){	
		//static EventSystemHandler instance;
		return instance;
	}*/
	static EventSystemHandler* getInstance(){
		if (instance == 0){ instance = new EventSystemHandler();}
		return instance;
	}
	void emitEvent(Event *e);
	void addObserver(Observer *o, EventTypes::EventType type);

	template <typename ET>
	void addObserver(TypedObserver<ET> *o) {
		list[ET().getType()].add(o);
	}
};
#endif
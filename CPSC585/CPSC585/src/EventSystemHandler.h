#include "Event.h"
#include "Observer.h"
#include "ObservingList.h"

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
	void addObserver(Observer *o);
};
#endif
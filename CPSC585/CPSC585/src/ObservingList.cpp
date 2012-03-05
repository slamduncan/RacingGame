#include "ObservingList.h"
#include "EventSystemHandler.h"


ObservingList::ObservingList()
{
	list = std::vector<Observer*>();
}

ObservingList::ObservingList(std::vector<Observer*> inList){
	list = inList;
}

void ObservingList::add(Observer* o){
	list.push_back(o);
}

void ObservingList::updateAll(Event *e){
	for (int i = 0; i < (int)list.size(); i++){
		list[i]->Observe(e);
	}
	//delete e;
}

int ObservingList::size(){ return list.size();}

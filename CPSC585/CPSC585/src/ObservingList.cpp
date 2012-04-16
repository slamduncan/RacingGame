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
		if (list[i] != 0)
			list[i]->Observe(e);
	}
	//delete e;
}

void ObservingList::remove(Observer* o){
	std::vector<Observer*> tempList;
	for (int i = 0; i < (int)list.size(); i++){
		if (list[i] != o)
		{
			tempList.push_back(o);
		}
	}
	list = tempList;
}

int ObservingList::size(){ return list.size();}

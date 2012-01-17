#ifndef OBSERVINGLIST_H
#define OBSERVINGLIST_H

#include "Observer.h"
#include <vector>

class ObservingList{
public:
	ObservingList();
	ObservingList(std::vector<Observer*> inList);
	void add(Observer* o);
	int size();
	void updateAll(Event *e);

private:
	std::vector<Observer*> list;
};
#endif
#pragma once
#include "Station.h";
class Event
{
protected:
	int timestep;
	int id;
public:
	Event(int timestep, int id) {
		this->timestep = timestep;
		this->id = id;
	}
	virtual void execute(LinkedList<Station*> stations) = 0;
	int getTimestep() {
		return timestep;
	}
};


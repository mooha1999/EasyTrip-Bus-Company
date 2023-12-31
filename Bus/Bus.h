#pragma once
#include "DataStructures/PriorityQueue.h"
#include "Passenger.h"
int id = 0;
class Bus
{
private:
	bool isMixed;
	int capacity, journeysToCheckup;
	int maintenanceTime;
	int movingTime;
	bool isForward;
	int currentStation;
	int journeys;
	bool isCheckup;
	PriorityQueue<Passenger*> passengers;

public:
	Bus(bool isMixed, int capacity, int journeysToCheckup)
		: isMixed(isMixed), capacity(capacity), journeysToCheckup(journeysToCheckup)
	{
		isForward = true;
		maintenanceTime = movingTime = journeys = 0;
		currentStation = 0;
		isCheckup = false;
		id++;
	}

	int getId()
	{
		return id;
	}

	int getCapacity()
	{
		return capacity;
	}

	int getMaintenanceTime()
	{
		return maintenanceTime;
	}

	int getMovingTime()
	{
		return movingTime;
	}

	bool IsForward()
	{
		return isForward;
	}

	bool IsMixed()
	{
		return isMixed;
	}

	int getCurrentStation()
	{
		return currentStation;
	}

	int getJourneys()
	{
		return journeys;
	}

	bool IsCheckup()
	{
		return isCheckup;
	}

	void setIsCheckup(bool isCheckup)
	{
		this->isCheckup = isCheckup;
	}

	void setMaintenanceTime(int maintenanceTime)
	{
		this->maintenanceTime = maintenanceTime;
	}

	void setIsForward(bool isForward)
	{
		this->isForward = isForward;
	}

	void setMovingTime(int movingTime)
	{
		this->movingTime = movingTime;
	}

	void setCurrentStation(int currentStation)
	{
		this->currentStation = currentStation;
	}

	void resetJourneys()
	{
		journeys = 0;
	}

	void addPassenger(Passenger* passenger)
	{
		if (passengers.Size() == capacity) {
			return;
		}
		int endStation = passenger->getEndStation();
		if (isForward) {
			passengers.Insert(passenger, endStation);
		}
		else {
			passengers.Insert(passenger, endStation * -1);
		}
	}

	Passenger* removePassenger()
	{
		return passengers.IsEmpty() ? nullptr : passengers.Dequeue();
	}

	void incrementJourneys()
	{
		journeys++;
		if (journeys == journeysToCheckup) {
			isCheckup = true;
			journeys = 0;
		}
	}

	bool canAddPassenger() {
		return passengers.Size() < capacity;
	}

	bool shouldOffboardPassenger() {
		if (passengers.IsEmpty()) {
			return false;
		}
		int nextStation = isForward ? currentStation + 1 : currentStation - 1;
		return passengers.Top()->getEndStation() == nextStation;
	}

	string info() {
		string ret = "B" + to_string(id) + "[";

		if (isForward) {
			ret += "FWD, ";
		}
		else{
			ret += "BCK, ";
		}

		if (isMixed) {
			ret += "MBus, ";
		}
		else {
			ret += "WBus, ";
		}
		ret += to_string(capacity) + "] {";
		for (Passenger* p : passengers) {
			ret += to_string(p->getId()) + ", ";
		}
		ret += "}\n";

		return ret;
	}
};

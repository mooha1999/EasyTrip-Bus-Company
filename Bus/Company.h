#pragma once
#include "DataStructures/LinkedList.h"
#include "LeaveEvent.h"
#include "ArrivalEvent.h"
#include "Station.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include "UI.h"
using namespace std;
class Company
{
	LinkedList<Station*> stations;
	Queue<Event*> events;
	Queue<Passenger*> finishedPassengers;
	UI ui;
	int maxWaitingTime;
	Event* createArrivalEvent(ifstream& file) {
		string passengerType;
		int hour, minute, id, startStation, endStation, priority = -1;
		char colon;
		file >> passengerType >> hour >> colon >> minute >> id >> startStation >> endStation;
		if (passengerType == "SP") {
			string subType;
			file >> subType;
			if (subType == "aged")
				priority = 2;
			else if (subType == "POD")
				priority = 1;
			else
				priority = 0;
		}
		int timestep = hour * 60 + minute;
		return new ArrivalEvent(timestep, id, passengerType, startStation, endStation, priority);
	}
	Event* createLeaveEvent(ifstream& file) {
		int hour, minute, id;
		char colon;
		file >> hour >> colon >> minute >> id;
		int timestep = hour * 60 + minute;
		return new LeaveEvent(timestep, id);
		
	}
	
	void randomAssigning(int timestep)
	{
		for (auto station : stations) {
			int randomNumber = rand() % 100 + 1;
			Passenger* passenger = nullptr;
			if (randomNumber < 21) {
				passenger = station->removeTopForwardSpecialPassenger();
				if (!passenger) {
					passenger = station->removeTopBackwardSpecialPassenger();
				}
			}
			else if (randomNumber < 31) {
				passenger = station->removeTopForwardWheelPassenger();
				if (!passenger) {
					passenger = station->removeTopBackwardWheelPassenger();
				}
			}
			else if (randomNumber < 61) {
				passenger = station->removeTopForwardNormalPassenger();
				if (!passenger) {
					passenger = station->removeTopBackwardNormalPassenger();
				}
			}
			if (passenger) {
				passenger->setFinishTime(timestep);
				passenger->setMovingTime(timestep);
				finishedPassengers.Push(passenger);
			}
		}
	}
	
	bool isAllListsEmpty() {
		for (auto station : stations) {
			if (!station->hasWaitingPassengers())
				return false;
		}
		return events.IsEmpty();
	}
public:
	Company() {
		maxWaitingTime = -1;
		stations = LinkedList<Station*>();
	}
	void readInputFile(string fileName) {
		ifstream file(fileName);
		//read number of stations
		int numberOfStations;
		file >> numberOfStations;
		numberOfStations++;
		while (numberOfStations--) {
			stations.InsertEnd(new Station());
		}
		//read max waiting time
		file >> maxWaitingTime;
		//read events
		int numberOfEvents;
		file >> numberOfEvents;
		while (numberOfEvents--) {
			char eventType;
			file >> eventType;
			Event* tempEvent = eventType == 'A'? createArrivalEvent(file) : createLeaveEvent(file);
			events.Push(tempEvent);
		}
		file.close();
	}

	void startSimulation() {
		int timestep = 0;
		while (!isAllListsEmpty()) {
			while (!events.IsEmpty() && events.Peek()->getTimestep() == timestep) {
				events.Pop()->execute(stations);
			}

			for (auto station : stations) {
				station->promotePassengers(timestep, maxWaitingTime);
			}

			randomAssigning(timestep);
			
			timestep++;
			ui.printSimulationInfo(timestep, stations, finishedPassengers);
		}
	}
	
};


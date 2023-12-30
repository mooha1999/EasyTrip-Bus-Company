#pragma once
#include "DataStructures/LinkedList.h"
#include "LeaveEvent.h"
#include "ArrivalEvent.h"
#include "Station.h"
#include "Bus.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include "UI.h"
using namespace std;
class Company
{
	Station* stations;
	Queue<Event*> events;
	Queue<Passenger*> finishedPassengers;
	Queue<Bus*> checkupWheelBuses;
	Queue<Bus*> checkupMixedBuses;
	Queue<Bus*> movingBuses;
	Queue<Bus*> stationZero;

	UI ui;

	int maxWaitingTime;
	int promotedPassengers;
	int numberOfStations;
	int timeBetweenStations;
	int journeysBeforeCheckup, wBusCheckupPeriod, mBusCheckupPeriod;
	int gettingTime;

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
		return new LeaveEvent(timestep, id, numberOfStations);
		
	}
	
	void randomAssigning(int timestep)
	{
		for (int i = 0; i < numberOfStations; i++) {
			int randomNumber = rand() % 100 + 1;
			Passenger* passenger = nullptr;
			if (randomNumber < 21) {
				passenger = stations[i].removeTopForwardSpecialPassenger();
				if (!passenger) {
					passenger = stations[i].removeTopBackwardSpecialPassenger();
				}
			}
			else if (randomNumber < 31) {
				passenger = stations[i].removeTopForwardWheelPassenger();
				if (!passenger) {
					passenger = stations[i].removeTopBackwardWheelPassenger();
				}
			}
			else if (randomNumber < 61) {
				passenger = stations[i].removeTopForwardNormalPassenger();
				if (!passenger) {
					passenger = stations[i].removeTopBackwardNormalPassenger();
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
		for (int i = 0; i < numberOfStations; i++) {
			Station station = stations[i];
			if (!stations[i].hasWaitingPassengers())
				return false;
		}
		return events.IsEmpty();
	}

	void generateOutputFile()
	{
		ofstream file("output.txt");
		file << "FT\t\t\tID\t\t\tAT\t\t\tWT\n";
		int npCount = 0,
			spCount = 0,
			wpCount = 0;
		int totalWaitingTime = 0;
		while (!finishedPassengers.IsEmpty()) {
			Passenger* passenger = finishedPassengers.Pop();

			file << timestepToHHMM(passenger->getFinishTime()) << "\t\t\t" << passenger->getId() << "\t\t\t";
			file << timestepToHHMM(passenger->getArrivalTime()) << "\t\t\t" << timestepToHHMM(passenger->getWaitingTime()) << '\n';

			totalWaitingTime += passenger->getWaitingTime();

			string type = passenger->getType();
			if (type == "NP")
				npCount++;
			else if (type == "SP")
				spCount++;
			else
				wpCount++;
		}
		float totalPAssengersCount = npCount + spCount + wpCount;
		file << "Passengers: " << totalPAssengersCount << "   [NP: " << npCount << ", SP: " << spCount << ", WP: " << wpCount << "]\n";
		file << "passenger Avg Wait time= " << timestepToHHMM(totalWaitingTime / totalPAssengersCount) << "\n";
		file << "Auto-promoted passengers: " << (float)promotedPassengers / totalPAssengersCount * 100.0 << "%\n";
	}

	string timestepToHHMM(int timestep) {
		string hour = to_string(timestep / 60);
		string minute = to_string(timestep % 60);
		return hour + ":" + minute;
	}

	void releaseBus(int timestep) {
		if (timestep < 241) { // 241 is equivilant to 4:01, p.s: during working hours
			return;
		}
		int workingMinutes = timestep - 240;
		if (workingMinutes % 15 == 0 && !stationZero.IsEmpty()) {
			Bus* bus = stationZero.Pop();
			bus->setMovingTime(timestep);
			movingBuses.Push(bus);
		}
	}

	void moveBuses(int timestep) {
		while (!movingBuses.IsEmpty() && timestep - movingBuses.Peek()->getMovingTime() == timeBetweenStations) {
			Bus* bus = movingBuses.Pop();
			if (bus->getCurrentStation() == 0 && !bus->IsForward()) {
				bus->incrementJourneys();
				addToCheckup(bus);
			}
		}
	}

	void addToCheckup(Bus* bus) {
		if (bus->getJourneys() == journeysBeforeCheckup) {
			bus->setIsCheckup(true);
			bus->resetJourneys();
			if (bus->getCurrentStation() == 0) {
				if (bus->IsMixed()) {
					checkupMixedBuses.Push(bus);
				}
				else {
					checkupWheelBuses.Push(bus);
				}
			}
			else {
				//TODO reverse the bus's direction and add it to moving list

			}
		}
	}
public:
	Company() {
		stations = nullptr;
		timeBetweenStations = maxWaitingTime = -1;
		journeysBeforeCheckup = wBusCheckupPeriod = mBusCheckupPeriod = -1;
		promotedPassengers = numberOfStations = 0;
		gettingTime = -1;
	}

	void readInputFile(string fileName) {
		ifstream file(fileName);
		//read number of stations
		file >> numberOfStations >> timeBetweenStations;
		stations = new Station[numberOfStations];
		for (int i = 0; i < numberOfStations; i++) {
			stations[i] = Station();
		}
		//read how many buses
		int wBusCount, mBusCount;
		file >> wBusCount >> mBusCount;
		//read buses capacities
		int wBusCapacity, mBusCapacity;
		file >> wBusCapacity >> mBusCapacity;
		//read checkup info
		file >> journeysBeforeCheckup >> wBusCheckupPeriod >> mBusCheckupPeriod;
		for (int i = 0, j = 0; i < wBusCount || j < mBusCount; i++, j++) {
			if (i < wBusCount) {
				stationZero.Push(new Bus(false, wBusCapacity));
			}
			if (i < mBusCount) {
				stationZero.Push(new Bus(true, mBusCapacity));
			}
		}
		//read max waiting time & getting on/off time
		file >> maxWaitingTime >> gettingTime;
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
		ui.getMode();
		int timestep = 0;
		while (!isAllListsEmpty() && timestep < 1439) {
			while (!events.IsEmpty() && events.Peek()->getTimestep() == timestep) {
				events.Pop()->execute(stations);
			}

			releaseBus(timestep);

			//handle moving buses
			while (!movingBuses.IsEmpty() && timestep - movingBuses.Peek()->getMovingTime() == timeBetweenStations) {
				Bus* bus = movingBuses.Pop();
				if (bus->getCurrentStation() == numberOfStations) {
					bus->setMovingTime(timestep);
					bus->setIsForward(false);
					bus->incrementJourneys();
					if (bus->getJourneys() == journeysBeforeCheckup) {
						bus->setIsCheckup(true);
						bus->resetJourneys();
					}
					movingBuses.Push(bus);
				}
				else if (bus->getCurrentStation() == 0 && !bus->IsForward()) {

				}
			}

			for (int i = 0; i < numberOfStations; i++) {
				promotedPassengers += stations[i].promotePassengers(timestep, maxWaitingTime);
			}

			randomAssigning(timestep);

			
			timestep++;
			ui.printSimulationInfo(timestep, stations, numberOfStations, finishedPassengers);
		}
		generateOutputFile();
		ui.displayEndMessage();
	}
};


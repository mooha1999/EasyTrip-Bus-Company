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
	Queue<Passenger*> completedPassengers;
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
	int boardingTime;

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
		while (!completedPassengers.IsEmpty()) {
			Passenger* passenger = completedPassengers.Dequeue();

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

	void releaseBusFromStationZero(int timestep) {
		if (timestep < 241) { // 241 is equivilant to 4:01, p.s: during working hours
			return;
		}
		int workingMinutes = timestep - 240;
		if (workingMinutes % 1 == 0 && !stationZero.IsEmpty()) {
			Bus* bus = stationZero.Dequeue();
			bus->setMovingTime(timestep);

			stations[0].addBus(bus);
		}
	}

	void addBusesToStations(int timestep)
	{
		//check if the difference between the current time and the moving time of the first bus in moving list is equal to timeBetweenStations
		while (!movingBuses.IsEmpty() && timestep - movingBuses.Top()->getMovingTime() == timeBetweenStations) {
			Bus* bus = movingBuses.Dequeue();
			int currentStation = bus->getCurrentStation();

			if (bus->IsCheckup() && !bus->IsForward() && currentStation == 1) {
				addToCheckup(bus, timestep);
			}
			else
			{
				int nextStation = bus->IsForward() ? 1 : -1;
				currentStation = currentStation + nextStation;
				bus->setCurrentStation(currentStation);
				if (currentStation == numberOfStations - 1) {
					stations[currentStation].addBus(bus);
				}
				if (!bus->IsCheckup() && (stations[currentStation].shouldAddBus(bus) || bus->shouldOffboardPassenger())) {
					stations[currentStation].addBus(bus);
				}
				else {
					bus->setMovingTime(timestep);
					movingBuses.Enqueu(bus);
				}
			}
		}
	}

	void addToCheckup(Bus* bus, int timestep)
	{
		bus->setMaintenanceTime(timestep);
		if (bus->IsMixed()) {
			checkupMixedBuses.Enqueu(bus);
		}
		else {
			checkupWheelBuses.Enqueu(bus);
		}
	}

	void releaseFromCheckup(Queue<Bus*>& checkupBuses, int checkupTime, int timestep) {
		while (!checkupBuses.IsEmpty() && timestep - checkupBuses.Top()->getMaintenanceTime() == checkupTime) {
			Bus* bus = checkupBuses.Dequeue();
			bus->setIsCheckup(false);
			bus->setIsForward(true);
			stations[0].addBus(bus);
		}
	}

	void executeCurrentEvent(int timestep)
	{
		if (timestep > 1320) {//equivialnt to 22:00, after working hours
			return;
		}
		while (!events.IsEmpty() && events.Top()->getTimestep() == timestep) {//TODO RETURN TO 15
			events.Dequeue()->execute(stations);
		}
	}

	void promotePassengers(int timestep)
	{
		for (int i = 0; i < numberOfStations; i++) {
			promotedPassengers += stations[i].promotePassengers(timestep, maxWaitingTime);
		}
	}

	void boardPassengers(int timestep)
	{
		for (int i = 0; i < numberOfStations; i++) {
			//offboard
			int timer1 = stations[i].offboardForward(completedPassengers, boardingTime, timestep);
			int timer2 = stations[i].offboardBackward(completedPassengers, boardingTime, timestep);
			//check checkup
			if (i == 0) {
				Queue<Bus*> checkupBuses = stations[0].handleFirstStationBuses();
				while (!checkupBuses.IsEmpty()) {
					Bus* bus = checkupBuses.Dequeue();
					addToCheckup(bus, timestep);
				}
			}
			else if (i == numberOfStations - 1) {
				Queue<Bus*> checkupBuses = stations[i].handleLastStation();
				while (!checkupBuses.IsEmpty()) {
					Bus* bus = checkupBuses.Dequeue();
					bus->setMovingTime(timestep);
					movingBuses.Enqueu(bus);
				}
			}
			if (timestep < 1320) {
				//onboard
				stations[i].onboardForward(timer1, timestep, boardingTime);
				stations[i].onboardBackward(timer2, timestep, boardingTime);
			}
			//move to Moving list
			Bus* bus = stations[i].removeForwardBus();
			if (bus) {
				bus->setMovingTime(timestep);
				movingBuses.Enqueu(bus);
			}
			bus = stations[i].removeBackwardBus();
			if (bus) {
				bus->setMovingTime(timestep);
				movingBuses.Enqueu(bus);
			}
		}
	}


public:
	Company() {
		stations = nullptr;
		timeBetweenStations = maxWaitingTime = -1;
		journeysBeforeCheckup = wBusCheckupPeriod = mBusCheckupPeriod = -1;
		promotedPassengers = numberOfStations = 0;
		boardingTime = -1;
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
		//add buses to station zero
		for (int i = 0, j = 0; i < wBusCount || j < mBusCount; i++, j++) {
			if (i < wBusCount) {
				stationZero.Enqueu(new Bus(false, wBusCapacity, journeysBeforeCheckup));
			}
			if (i < mBusCount) {
				stationZero.Enqueu(new Bus(true, mBusCapacity, journeysBeforeCheckup));
			}
		}
		//read max waiting time & getting on/off time
		file >> maxWaitingTime >> boardingTime;
		//read events
		int numberOfEvents;
		file >> numberOfEvents;
		//fill the events queue
		while (numberOfEvents--) {
			char eventType;
			file >> eventType;
			Event* tempEvent = eventType == 'A' ? createArrivalEvent(file) : createLeaveEvent(file);
			events.Enqueu(tempEvent);
		}
		file.close();
	}

	void startSimulation() {
		ui.getMode();
		int timestep = 239;
		while (timestep < 1439) {
			releaseBusFromStationZero(timestep);

			releaseFromCheckup(checkupMixedBuses, mBusCheckupPeriod, timestep);
			releaseFromCheckup(checkupWheelBuses, wBusCheckupPeriod, timestep);

			addBusesToStations(timestep);

			executeCurrentEvent(timestep);

			promotePassengers(timestep);

			boardPassengers(timestep);

			timestep++;
			ui.printSimulationInfo(timestep, stations, numberOfStations, completedPassengers, checkupWheelBuses, checkupMixedBuses, movingBuses);
		}
		generateOutputFile();
		ui.displayEndMessage();
	}



};


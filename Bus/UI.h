#pragma once
#include "Station.h"
#include <iostream>
using namespace std;
class UI
{
	bool isSilent;
public:
	UI() {
		isSilent = true;
	}

	void getMode() {
		cout << "Simulation started" << endl << "Choose mode:\n[1] Interactive\n[2] Silent\n";
		int mode;
		cin >> mode;
		if (mode == 1) {
			isSilent = false;
		}
	}

	void printSimulationInfo(int timestep, Station* stations, int numberOfStations, Queue<Passenger*>& finishedPassengers, Queue<Bus*> checkupWheelBuses, Queue<Bus*> checkupMixedBuses, Queue<Bus*> movingBuses) {
		if (isSilent) {
			return;
		}

		cout << "Current Time (Hour:Min) ==> " << timestep / 60 << ":" << timestep % 60 << endl;
		for (int i = 0; i < numberOfStations; i++) {
			Station station = stations[i];
			cout << "============== STATION #" << i + 1 << "=================\n" << endl;
			cout << station.info();
		}
		cout << "\n-------------------------------------------------------\n";
		int checkupCount = checkupMixedBuses.Size() + checkupWheelBuses.Size();
		cout << checkupCount << " In-Checkup buses: ";
		for (Bus* bus : checkupMixedBuses) {
			cout << bus->getId() << ", ";
		}
		for (Bus* bus : checkupWheelBuses) {
			cout << bus->getId() << ", ";
		}
		cout << "\n-------------------------------------------------------\n";
		cout << finishedPassengers.Size() << " finished passengers: ";
		for (auto passenger : finishedPassengers) {
			cout << passenger->getId() << ", ";
		}
		cout << endl;
		system("pause");

	}

	void displayEndMessage() {
		cout << "Simulation ended\nOutput file created\n";
	}
};


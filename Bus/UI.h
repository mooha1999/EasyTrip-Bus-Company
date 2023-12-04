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

	void printSimulationInfo(int timestep, LinkedList<Station*>& stations, Queue<Passenger*>& finishedPassengers) {
		if (isSilent) {
			return;
		}

		cout << "Current Time (Hour:Min) ==> " << timestep / 60 << ":" << timestep % 60 << endl;
		int i = 0;
		for (auto station : stations) {
			if (i == 0) {
				i++;
				continue;
			}
			cout << "============== STATION #" << i << "=================\n" << endl;
			cout << station->info();
			i++;
			cout << "Press [Enter] to display next station\n";
			getchar();
		}
		cout << "\n-------------------------------------------------------\n";
		cout << finishedPassengers.Size() << " finished passengers: ";
		for (auto passenger : finishedPassengers) {
			cout << passenger->getId() << ", ";
		}
		cout << endl;
	}

	void displayEndMessage() {
		cout << "Simulation ended\nOutput file created\n";
	}
};


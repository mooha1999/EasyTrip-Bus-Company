#pragma once
#include "Station.h"
#include <iostream>
using namespace std;
class UI
{
public:
	void printSimulationInfo(int timestep, LinkedList<Station*>& stations, Queue<Passenger*>& finishedPassengers) {
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
			cout << "Press any key to display next station\n";
		}
		cout << "\n-------------------------------------------------------\n";
		cout << finishedPassengers.Size() << " finished passengers: ";
		for (auto passenger : finishedPassengers) {
			cout << passenger->getId() << ", ";
		}
		cout << endl;
	}
};


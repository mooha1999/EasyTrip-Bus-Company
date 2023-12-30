#pragma once
#include "DataStructures/LinkedList.h"
#include "DataStructures/Queue.h"
#include "DataStructures/PriorityQueue.h"
#include "Passenger.h"
#include "Bus.h"
class Station
{
	LinkedList<Passenger*> waitingForwardNormalPassengers;
	LinkedList<Passenger*> waitingBackwardNormalPassengers;
	PriorityQueue<Passenger*> waitingForwardSpecialPassengers;
	PriorityQueue<Passenger*> waitingBackwardSpecialPassengers;
	Queue<Passenger*> waitingForwardWheelPassengers;
	Queue<Passenger*> waitingBackwardWheelPassengers;

	Queue<Bus*> forwardBuses;
	Queue<Bus*> backwardBuses;
public:
	void addPassenger(Passenger* passenger) {
		string passengerType = passenger->getType();

		// Check the type of passenger and add to the corresponding queue/priority queue
		if (passengerType == "NP") {
			// Normal Passenger
			if (passenger->getStartStation() < passenger->getEndStation()) {
				waitingForwardNormalPassengers.InsertEnd(passenger);
			}
			else {
				waitingBackwardNormalPassengers.InsertEnd(passenger);
			}
		}
		else if (passengerType == "SP") {
			// Special Passenger
			if (passenger->getStartStation() < passenger->getEndStation()) {
				waitingForwardSpecialPassengers.Insert(passenger, passenger->getPriority());
			}
			else {
				waitingBackwardSpecialPassengers.Insert(passenger, passenger->getPriority());
			}
		}
		else if (passengerType == "WP") {
			// Wheel Passenger
			if (passenger->getStartStation() < passenger->getEndStation()) {
				waitingForwardWheelPassengers.Push(passenger);
			}
			else {
				waitingBackwardWheelPassengers.Push(passenger);
			}
		}
	}

	int promotePassengers(int timestep, int maxWaitingTime) {
		int promotedPassengersCount = 0;
		Queue<Passenger*> promotedPassengers;
		for (auto passenger : waitingForwardNormalPassengers) {
			int passengerWaitingTime = timestep - passenger->getArrivalTime();
			if (passengerWaitingTime > maxWaitingTime) {
				promotedPassengersCount++;
				promotedPassengers.Push(passenger);
			}
		}
		while (!promotedPassengers.IsEmpty()) {
			Passenger* passenger = promotedPassengers.Pop();
			waitingForwardNormalPassengers.Remove(passenger);
			passenger->setPriority(2);
			waitingForwardSpecialPassengers.Insert(passenger, 2);
		}

		for (auto passenger : waitingBackwardNormalPassengers) {
			int passengerWaitingTime = timestep - passenger->getArrivalTime();
			if (passengerWaitingTime > maxWaitingTime) {
				promotedPassengersCount++;
				promotedPassengers.Push(passenger);
			}
		}
		while (!promotedPassengers.IsEmpty()) {
			Passenger* passenger = promotedPassengers.Pop();
			waitingBackwardNormalPassengers.Remove(passenger);
			passenger->setPriority(2);
			waitingBackwardSpecialPassengers.Insert(passenger, 2);
		}

		return promotedPassengersCount;
	}

	void addBus(Bus* bus) {
		if (bus->IsForward()) {
			forwardBuses.Push(bus);
		}
		else {
			backwardBuses.Push(bus);
		}
	}

	Queue<Bus*> removeBuses() {

	}

	void onboardPassengers() {

	}

	void passengerLeave(int id) {
		for (auto p : waitingBackwardNormalPassengers) {
			if (p->getId() == id) {
				waitingBackwardNormalPassengers.Remove(p);
				return;
			}
		}
		for (auto p : waitingForwardNormalPassengers) {
			if (p->getId() == id) {
				waitingForwardNormalPassengers.Remove(p);
				return;
			}
		}
	}

	bool hasWaitingPassengers()
	{
		return waitingForwardNormalPassengers.IsEmpty() &&
			waitingBackwardNormalPassengers.IsEmpty() &&
			waitingForwardSpecialPassengers.IsEmpty() &&
			waitingBackwardSpecialPassengers.IsEmpty() &&
			waitingForwardWheelPassengers.IsEmpty() &&
			waitingBackwardWheelPassengers.IsEmpty();
	}

	Queue<Bus*> handlePassengers(Queue<Passenger*>& completedPassengers, int boardingTime, bool isLastStation) {
		Queue<Bus*> movedBuses;// buses that will be moved to the moving list
		int timer = 0;
		while (!forwardBuses.IsEmpty() && timer < 60)
		{
			Bus* bus = forwardBuses.Peek();
			Passenger* passenger = bus->removePassenger();
			while (passenger && timer < 60) {
				completedPassengers.Push(passenger);
				passenger = bus->removePassenger();
				timer += boardingTime;
			}
			if (timer > 60)
				break;
			if (isLastStation) {
				forwardBuses.Pop();
				bus->incrementJourneys();
				bus->setIsForward(false);
				if (bus->IsCheckup()) {
					movedBuses.Push(bus);
				}
				else {
					backwardBuses.Push(bus);
				}
			}
			else {
				if (bus->IsMixed()) {
					while (!waitingForwardSpecialPassengers.IsEmpty() && bus->canAddPassenger() && timer < 60) {
						Passenger* passenger = waitingForwardSpecialPassengers.Pop();
						bus->addPassenger(passenger);
						timer += boardingTime;
					}
					while (!waitingForwardNormalPassengers.IsEmpty() && bus->canAddPassenger() && timer < 60) {
						Passenger* passenger = waitingForwardNormalPassengers.RemoveAt(0);
						bus->addPassenger(passenger);
						timer += boardingTime;
					}
				}
			}
		}
	}

	string info() {
		string ret = "";

		string types[3] = { "pregnant", "POD", "aged" };

		int spCount = waitingBackwardSpecialPassengers.Size() + waitingForwardSpecialPassengers.Size(),
			wpCount = waitingBackwardWheelPassengers.Size() + waitingForwardWheelPassengers.Size(),
			npCount = waitingBackwardNormalPassengers.Size() + waitingForwardNormalPassengers.Size();
		// start SP
		ret += to_string(spCount) + " Waiting SP: FWD[";

		for (auto passenger : waitingForwardSpecialPassengers) {
			ret += to_string(passenger->getId()) + "(" + types[passenger->getPriority()] + "), ";
		}

		ret += "] BCK[";

		for (auto passenger : waitingBackwardSpecialPassengers) {
			ret += to_string(passenger->getId()) + "(" + types[passenger->getPriority()] + "), ";
		}

		ret += "]\n";
		// end SP

		// start WP
		ret += to_string(wpCount) + " Waiting WP: FWD[";

		for (auto passenger : waitingForwardWheelPassengers) {
			ret += to_string(passenger->getId()) + ", ";
		}

		ret += "] BCK[";

		for (auto passenger : waitingBackwardWheelPassengers) {
			ret += to_string(passenger->getId()) + "(, ";
		}

		ret += "]\n";
		// end WP

		//start NP
		ret += to_string(npCount) + " Waiting NP: FWD[";

		for (auto passenger : waitingForwardNormalPassengers) {
			ret += to_string(passenger->getId()) + ", ";
		}

		ret += "] BCK[";

		for (auto passenger : waitingBackwardNormalPassengers) {
			ret += to_string(passenger->getId()) + "(, ";
		}

		ret += "]\n";
		//end NP
		return ret;
	}

};


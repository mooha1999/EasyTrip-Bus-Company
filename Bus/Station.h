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

	Passenger* nextPassenger(Bus* bus) {
		if (bus->IsMixed()) {
			if (bus->IsForward()) {
				if (!waitingForwardSpecialPassengers.IsEmpty()) {
					return waitingForwardSpecialPassengers.Dequeue();
				}
				else if (!waitingForwardNormalPassengers.IsEmpty()) {
					return waitingForwardNormalPassengers.RemoveAt(0);
				}
				return nullptr;
			}
			else {
				if (!waitingBackwardSpecialPassengers.IsEmpty()) {
					return waitingBackwardSpecialPassengers.Dequeue();
				}
				else if (!waitingBackwardNormalPassengers.IsEmpty()) {
					return waitingBackwardNormalPassengers.RemoveAt(0);
				}
				return nullptr;
			}
		}
		else {
			if (bus->IsForward()) {
				if (!waitingForwardWheelPassengers.IsEmpty()) {
					return waitingForwardWheelPassengers.Dequeue();
				}
				return nullptr;
			}
			else {
				if (!waitingBackwardWheelPassengers.IsEmpty()) {
					return waitingBackwardWheelPassengers.Dequeue();
				}
				return nullptr;
			}
		}
	}

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
				waitingForwardWheelPassengers.Enqueu(passenger);
			}
			else {
				waitingBackwardWheelPassengers.Enqueu(passenger);
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
				promotedPassengers.Enqueu(passenger);
			}
		}
		while (!promotedPassengers.IsEmpty()) {
			Passenger* passenger = promotedPassengers.Dequeue();
			waitingForwardNormalPassengers.Remove(passenger);
			passenger->setPriority(2);
			waitingForwardSpecialPassengers.Insert(passenger, 2);
		}

		for (auto passenger : waitingBackwardNormalPassengers) {
			int passengerWaitingTime = timestep - passenger->getArrivalTime();
			if (passengerWaitingTime > maxWaitingTime) {
				promotedPassengersCount++;
				promotedPassengers.Enqueu(passenger);
			}
		}
		while (!promotedPassengers.IsEmpty()) {
			Passenger* passenger = promotedPassengers.Dequeue();
			waitingBackwardNormalPassengers.Remove(passenger);
			passenger->setPriority(2);
			waitingBackwardSpecialPassengers.Insert(passenger, 2);
		}

		return promotedPassengersCount;
	}

	void addBus(Bus* bus) {
		if (bus->IsForward()) {
			forwardBuses.Enqueu(bus);
		}
		else {
			backwardBuses.Enqueu(bus);
		}
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
			Bus* bus = forwardBuses.Top();
			Passenger* passenger = bus->removePassenger();
			while (passenger && timer < 60) {
				completedPassengers.Enqueu(passenger);
				passenger = bus->removePassenger();
				timer += boardingTime;
			}
			if (timer > 60)
				break;
			if (isLastStation) {
				forwardBuses.Dequeue();
				bus->incrementJourneys();
				bus->setIsForward(false);
				if (bus->IsCheckup()) {
					movedBuses.Enqueu(bus);
				}
				else {
					backwardBuses.Enqueu(bus);
				}
			}
			else {
				if (bus->IsMixed()) {
					while (!waitingForwardSpecialPassengers.IsEmpty() && bus->canAddPassenger() && timer < 60) {
						Passenger* passenger = waitingForwardSpecialPassengers.Dequeue();
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

	int offboardForward(Queue<Passenger*>& completedPassengers, int boardingTime) {
		if (forwardBuses.IsEmpty()) {
			return 0;
		}
		int timer = 0;
		Passenger* passenger = forwardBuses.Top()->removePassenger();
		while (!forwardBuses.IsEmpty() && passenger && timer < 60) {
			completedPassengers.Enqueu(passenger);
			passenger = forwardBuses.Top()->removePassenger();
			timer += boardingTime;
		}
		return timer;
	}

	int offboardBackward(Queue<Passenger*>& completedPassengers, int boardingTime) {
		if (backwardBuses.IsEmpty()) {
			return 0;
		}
		int timer = 0;
		Passenger* passenger = backwardBuses.Top()->removePassenger();
		while (!backwardBuses.IsEmpty() && passenger && timer < 60) {
			completedPassengers.Enqueu(passenger);
			passenger = backwardBuses.Top()->removePassenger();
			timer += boardingTime;
		}
		return timer;
	}

	Queue<Bus*> handleFirstStationBuses() {
		Queue<Bus*> checkupBuses;
		while (!backwardBuses.IsEmpty()) {
			Bus* bus = backwardBuses.Dequeue();
			bus->incrementJourneys();
			if (bus->IsCheckup()) {
				checkupBuses.Enqueu(bus);
			}
			else {
				bus->setIsForward(true);
				forwardBuses.Enqueu(bus);
			}
		}
		return checkupBuses;
	}

	Queue<Bus*> handleLastStation() {
		Queue<Bus*> checkupBuses;
		while (!forwardBuses.IsEmpty()) {
			Bus* bus = forwardBuses.Dequeue();
			bus->incrementJourneys();
			bus->setIsForward(false);
			if (bus->IsCheckup()) {
				checkupBuses.Enqueu(bus);
			}
			else {
				backwardBuses.Enqueu(bus);
			}
		}
		return checkupBuses;
	}

	void onboardForward(int timer, int timestep, int boardingTime) {
		if (forwardBuses.IsEmpty()) {
			return;
		}
		Passenger* passenger = nextPassenger(forwardBuses.Top());
		while (!forwardBuses.IsEmpty() && forwardBuses.Top()->canAddPassenger() && passenger && timer < 60) {
			Bus* bus = forwardBuses.Top();
			bus->addPassenger(passenger);
			passenger = nextPassenger(forwardBuses.Top());
			timer += boardingTime;
		}
	}

	void onboardBackward(int timer, int timestep, int boardingTime) {
		if (backwardBuses.IsEmpty()) {
			return;
		}
		Passenger* passenger = nextPassenger(backwardBuses.Top());
		while (!backwardBuses.IsEmpty() && backwardBuses.Top()->canAddPassenger() && passenger && timer < 60) {
			Bus* bus = backwardBuses.Top();
			bus->addPassenger(passenger);
			passenger = nextPassenger(backwardBuses.Top());
			timer += boardingTime;
		}
	}

	Bus* removeForwardBus() {
		if (!forwardBuses.IsEmpty() && (!forwardBuses.Top()->canAddPassenger() || !shouldAddBus(forwardBuses.Top()))) {
			return forwardBuses.Dequeue();
		}
		else return nullptr;
	}

	Bus* removeBackwardBus() {
		if (!backwardBuses.IsEmpty() && (!backwardBuses.Top()->canAddPassenger() || !shouldAddBus(backwardBuses.Top()))) {
			return backwardBuses.Dequeue();
		}
		else return nullptr;
	}

	bool shouldAddBus(Bus* bus) {
		if (bus->IsMixed()) {
			if (bus->IsForward()) {
				return !waitingForwardNormalPassengers.IsEmpty() || !waitingForwardSpecialPassengers.IsEmpty();
			}
			else {
				return !waitingBackwardNormalPassengers.IsEmpty() || !waitingBackwardSpecialPassengers.IsEmpty();
			}
		}
		else {
			if (bus->IsForward()) {
				return !waitingForwardWheelPassengers.IsEmpty();
			}
			else {
				return !waitingBackwardWheelPassengers.IsEmpty();
			}
		}
	}

	string info() {
		string ret = "";

		string types[3] = { "pregnant", "POD", "aged" };

		int spCount = waitingBackwardSpecialPassengers.Size() + waitingForwardSpecialPassengers.Size(),
			wpCount = waitingBackwardWheelPassengers.Size() + waitingForwardWheelPassengers.Size(),
			npCount = waitingBackwardNormalPassengers.Size() + waitingForwardNormalPassengers.Size(),
			busesCount = forwardBuses.Size() + backwardBuses.Size();
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
			ret += to_string(passenger->getId()) + ", ";
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

		//start buses
		ret += to_string(busesCount) + " Buses at the station:\n";
		for (Bus* bus : forwardBuses) {
			ret += bus->info();
		}
		for (Bus* bus : backwardBuses) {
			ret += bus->info();
		}
		return ret;
	}

};


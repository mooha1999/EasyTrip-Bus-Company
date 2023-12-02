#pragma once
#include "DataStructures/LinkedList.h"
#include "Passenger.h";
class Bus
{
private:
    bool isMixed;
    int capacity;
    int maintenanceTime;
    LinkedList<Passenger*> passengers;
public:
    Bus(bool isMixed, int capacity, int maintenanceTime)
        : isMixed(isMixed), capacity(capacity), maintenanceTime(maintenanceTime)
    {
    }

    bool getIsMixed()
    {
        return isMixed;
    }

    int getCapacity()
    {
        return capacity;
    }

    int getMaintenanceTime()
    {
        return maintenanceTime;
    }

    void addPasseneger(Passenger* passenger) {
        passengers.InsertEnd(passenger);
    }
    Passenger* removePassenger(int currentStation) {

        return passengers.IsEmpty() && passengers[0]->getEndStation() == currentStation ? nullptr : passengers.RemoveAt(0);
    }
};



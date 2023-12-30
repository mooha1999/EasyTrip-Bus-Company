#pragma once
#include "DataStructures/PriorityQueue.h"
#include "Passenger.h"

class Bus
{
private:
    bool isMixed;
    int capacity;
    int maintenanceTime;
    int movingTime;
    bool isForward;
    int currentStation;
    int journeys;
    bool isCheckup;
    PriorityQueue<Passenger*> passengers;

public:
    Bus(bool isMixed, int capacity)
        : isMixed(isMixed), capacity(capacity)
    {
        isForward = true;
        maintenanceTime = movingTime = journeys = 0;
        currentStation = -1;
        isCheckup = false;
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
        passengers.Insert(passenger, 1);
    }

    Passenger* removePassenger(int currentStation)
    {
        return passengers.IsEmpty() && passengers.Top()->getEndStation() == currentStation ? nullptr : passengers.Pop();
    }

    void incrementJourneys()
    {
        journeys++;
    }
};

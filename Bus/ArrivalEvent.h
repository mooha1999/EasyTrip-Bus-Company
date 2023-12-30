#pragma once
#include "Event.h"
#include <string>
using namespace std;
class ArrivalEvent :
    public Event
{
    string passenegerType;
    int startStation;
    int endStation;
    int priority;
public:
    ArrivalEvent(int timestep, int id, string passenegerType, int startStation, int endStation, int priority = -1)
        :Event(timestep, id),
        passenegerType(passenegerType), startStation(startStation - 1), endStation(endStation - 1), priority(priority) {}

    void execute(Station* stations) {

        Passenger* passenger = new Passenger(passenegerType, id, timestep, startStation, endStation, priority);

        stations[startStation].addPassenger(passenger);
    }
};


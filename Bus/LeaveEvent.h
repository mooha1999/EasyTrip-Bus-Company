#pragma once
#include "Event.h"
#include "DataStructures/LinkedList.h"
class LeaveEvent :
    public Event
{
    int numberOfStations;
public:
    LeaveEvent(int timestep, int id, int numberOfStations) : Event(timestep, id), numberOfStations(numberOfStations) {}
    void execute(Station* stations) {
        for (int i = 0; i < numberOfStations; i++) {
            stations[i].passengerLeave(id);
        }
    }
};


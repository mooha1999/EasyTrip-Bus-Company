#pragma once
#include "Event.h"
#include "DataStructures/LinkedList.h"
class LeaveEvent :
    public Event
{
public:
    LeaveEvent(int timestep, int id) : Event(timestep, id) {}
    void execute(LinkedList<Station*> stations) {
        for (auto station: stations) {
            station->passengerLeave(id);
        }
    }
};


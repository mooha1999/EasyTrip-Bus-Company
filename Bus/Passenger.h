#pragma once
#include <string>
using namespace std;
class Passenger
{
private:
    string type;
    int id;
    int arrivalTime;
    int startStation;
    int endStation;
    int priority;
    int gettingTime;
    int movingTime;
    int finishTime;
public:
    Passenger(string type, int id, int arrivalTime, int startStation, int endStation, int priority)
        : type(type), arrivalTime(arrivalTime), startStation(startStation), endStation(endStation), priority(priority), id(id)
    {
        gettingTime = movingTime = finishTime = -1;
    }
    /***GETTERS***/
    string getType()
    {
        return type;
    }

    int getId()
    {
        return id;
    }

    int getArrivalTime()
    {
        return arrivalTime;
    }

    int getStartStation()
    {
        return startStation;
    }

    int getEndStation()
    {
        return endStation;
    }

    int getPriority()
    {
        return priority;
    }

    int getGettingTime()
    {
        return gettingTime;
    }

    int getMovingTime()
    {
        return movingTime;
    }

    int getWaitingTime()
    {
        return movingTime - arrivalTime;
    }

    int getTripTime()
    {
        finishTime - movingTime;
    }

    /*int getFinishTime() const
    {
        return finishTime;
    }*/
    /***SETTERS***/
    void setMovingTime(int movingTime)
    {
        this->movingTime = movingTime;
    }

    void setFinishTime(int finishTime)
    {
        this->finishTime = finishTime;
    }

    void setGettingTime(int gettingTime)
    {
        this->gettingTime = gettingTime;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }
};



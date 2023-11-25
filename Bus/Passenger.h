#pragma once
class Passenger
{
private:
    int arrivalTime;
    int startStation;
    int endStation;
    int gettingTime;

public:
    Passenger(int arrivalTime, int startStation, int endStation, int gettingTime)
        : arrivalTime(arrivalTime), startStation(startStation), endStation(endStation), gettingTime(gettingTime)
    {
    }

    int getArrivalTime() const
    {
        return arrivalTime;
    }

    void setArrivalTime(int arrivalTime)
    {
        this->arrivalTime = arrivalTime;
    }

    int getStartStation() const
    {
        return startStation;
    }

    void setStartStation(int startStation)
    {
        this->startStation = startStation;
    }

    int getEndStation() const
    {
        return endStation;
    }

    void setEndStation(int endStation)
    {
        this->endStation = endStation;
    }

    int getGettingTime() const
    {
        return gettingTime;
    }

    void setGettingTime(int gettingTime)
    {
        this->gettingTime = gettingTime;
    }
};



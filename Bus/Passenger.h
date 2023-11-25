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

    int getGettingTime()
    {
        return gettingTime;
    }
};



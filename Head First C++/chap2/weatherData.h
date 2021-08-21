#ifndef __WEATHERDATA_H__
#define __WEATHERDATA_H__

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Observer {
public:
    virtual void update(float temp, float humidity, float pressure) = 0;

};

class DisplayElement {
public:
    virtual void display() = 0;

};

class Subject {
public:
    virtual void registerObserver(Observer *o) = 0;
    virtual void removeObserver(Observer *o) = 0;
    virtual void notifyObserver() = 0;
};

class WeatherData: public Subject {
public:
    WeatherData(): temperature(0), humidity(0), pressure(0) {};
    void registerObserver(Observer* o);
    void removeObserver(Observer* o);
    void notifyObserver();
    void measurementsChanged();
    void setMeasurements(float temperature, float humidity, float pressure);

private:
    vector<Observer*> observers;
    float temperature;
    float humidity;
    float pressure;

};

class CurrentConditionsDisplay: public Observer, public DisplayElement {
public:
    CurrentConditionsDisplay(Subject* weatherData);
    void update(float temperature, float humidity, float pressure);
    void display();

private:
    float temperature;
    float humidity;
    Subject *weatherData;

};

#endif
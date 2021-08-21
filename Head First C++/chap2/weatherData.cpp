
#include "weatherData.h"

using namespace std;

void WeatherData::registerObserver(Observer *o) {
    observers.push_back(o);
}

void WeatherData::removeObserver(Observer *o) {
    vector<Observer *>::iterator found = find(observers.begin(), observers.end(), o);
    if (found != observers.end())
    {
        observers.erase(found);
    }
}

void WeatherData::notifyObserver() {
    for (auto it:observers)
    {
        it->update(temperature, humidity, pressure);
    }
}

void WeatherData::measurementsChanged() {
    notifyObserver();
}

void WeatherData::setMeasurements(float temperature, float humidity, float pressure) {
    this->temperature = temperature;
    this->humidity = humidity;
    this->pressure = pressure;
    measurementsChanged();
}

CurrentConditionsDisplay::CurrentConditionsDisplay(Subject *weatherData) {
    this->weatherData = weatherData;
    weatherData->registerObserver(this);
}

void CurrentConditionsDisplay::update(float temperature, float humidity, float pressure) {
    this->temperature = temperature;
    this->humidity = humidity;
    display();
}

void CurrentConditionsDisplay::display() {
    cout << "Current conditions: " << temperature << "F degrees and " << humidity << "\% humidity" << endl;
}

int main() {
    WeatherData *weatherData = new WeatherData();
    CurrentConditionsDisplay *currentDisplay = new CurrentConditionsDisplay(weatherData);

    weatherData->setMeasurements(80, 65, 30.4);
    weatherData->setMeasurements(82, 70, 29.2);
    weatherData->setMeasurements(78, 90, 29.2);

    return 0;
}
#pragma once
#include <vector>
#include "Observer.h"
#include "Event.h"
class Subject {
public:
	void addObserver(Observer* observer) {
		observers.push_back(observer);
	}
protected:
	void notify(Event* event) {
		for (Observer* observer : observers) {
			observer->onNotify(event);
		}
	}
private:
	std::vector<Observer*> observers;
};

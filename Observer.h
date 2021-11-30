#pragma once
#include "Event.h"
class Observer {
public:
	virtual ~Observer() {}
	virtual void onNotify(Event* event) = 0;
};


#pragma once
#include "ComputerFactory.h"
#include "HardComputer.h"
#include "MediumComputer.h"
#include "EasyComputer.h"

Computer* ComputerFactory::InitComputer(const Computer::Difficulty& d) {
	Computer* c;
	switch (d) {
		case Computer::Difficulty::EASY:
			c = new EasyComputer(d);
			break;
		case Computer::Difficulty::MEDIUM:
			c = new MediumComputer(d);
			break;
		case Computer::Difficulty::HARD:
			c = new HardComputer(d);
			break;
		default:
			return nullptr;
			break;
	}
	return c;
}

void ComputerFactory::DeallocComputer(Computer* c) {
	delete c;
}
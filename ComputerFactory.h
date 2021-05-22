#pragma once
#include "Computer.h"

class ComputerFactory
{
	public:
		static Computer* InitComputer(const Computer::Difficulty& d);
		static void DeallocComputer(Computer* c);
};

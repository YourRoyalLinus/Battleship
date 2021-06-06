#pragma once
#include "Command.h"
class NullCommand : public Command {
	void execute() override {
		//do nothing!
	}
};

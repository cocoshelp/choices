#pragma once
#include "StatesInfo.h"

class State
{
public:
	static State* getInstance()
	{
		static State instance;
		return &instance;
	}

	StatesInfo getState();
	void setState(StatesInfo statesIndex);

private:
	StatesInfo _stateInfo;
	State() { _stateInfo = StatesInfo::Begin; }
	State(State const&);
};
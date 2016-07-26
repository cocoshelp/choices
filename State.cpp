#include "State.h"
#include <utility>

StatesInfo State::getState()
{
	return _stateInfo;
}

void State::setState(StatesInfo statesIndex)
{
	_stateInfo = statesIndex;
}

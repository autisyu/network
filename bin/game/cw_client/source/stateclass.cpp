////////////////////////////////////////////////////////////////////////////////
// Filename: stateclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stateclass.h"


StateClass::StateClass()
{
	m_currentState = STATE_BLACKFOREST;
}


StateClass::StateClass(const StateClass& other)
{
}


StateClass::~StateClass()
{
}


int StateClass::GetCurrentState()
{
	return m_currentState;
}
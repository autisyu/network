////////////////////////////////////////////////////////////////////////////////
// Filename: entitylistclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "entitylistclass.h"


EntityListClass::EntityListClass()
{
	m_online = false;
	m_movingForward = false;
	m_movingBackward = false;
	m_turningLeft = false;
	m_turningRight = false;
}


EntityListClass::EntityListClass(const EntityListClass& other)
{
}


EntityListClass::~EntityListClass()
{
}


void EntityListClass::SetOnlineStatus(bool online)
{
	m_online = online;
	return;
}


void EntityListClass::SetId(unsigned short id)
{
	m_entityId = id;
	return;
}


void EntityListClass::SetType(char type)
{
	m_entityType = type;
	return;
}


void EntityListClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void EntityListClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


bool EntityListClass::IsOnline()
{
	return m_online;
}


unsigned short EntityListClass::GetId()
{
	return m_entityId;
}


char EntityListClass::GetEntityType()
{
	return m_entityType;
}


void EntityListClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void EntityListClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void EntityListClass::UpdateState(char state)
{
	if(state == E_STATE_MOVING_FORWARD)       { m_movingForward = true; }
	if(state == E_STATE_NOT_MOVING_FORWARD)   { m_movingForward = false; }
	if(state == E_STATE_MOVING_BACKWARD)      { m_movingBackward = true; }
	if(state == E_STATE_NOT_MOVING_BACKWARD)  { m_movingBackward = false; }
	if(state == E_STATE_TURNING_LEFT)         { m_turningLeft = true; }
	if(state == E_STATE_NOT_TURNING_LEFT)     { m_turningLeft = false; }
	if(state == E_STATE_TURNING_RIGHT)        { m_turningRight = true; }
	if(state == E_STATE_NOT_TURNING_RIGHT)    { m_turningRight = false; }

	return;
}


void EntityListClass::Frame(float frameTime)
{
	float radians;


	if(m_movingForward)
	{
		radians = m_rotationY * 0.0174532925f;
		m_positionX += sinf(radians) * 0.3f;
		m_positionZ += cosf(radians) * 0.3f;
	}
		
	if(m_movingBackward)
	{
		radians = m_rotationY * 0.0174532925f;
		m_positionX -= sinf(radians) * 0.3f;
		m_positionZ -= cosf(radians) * 0.3f;
	}

	if(m_turningLeft)
	{
		m_rotationY -= 1.5f;
		if(m_rotationY < 0.0f)
		{
			m_rotationY += 360.0f;
		}
	}

	if(m_turningRight)
	{
		m_rotationY += 1.5f;
		if(m_rotationY > 360.0f)
		{
			m_rotationY -= 360.0f;
		}
	}

	return;
}


void EntityListClass::UpdatePosition(float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ)
{
	m_positionX = positionX;
	m_positionY = positionY;
	m_positionZ = positionZ;

	m_rotationX = rotationX;
	m_rotationY = rotationY;
	m_rotationZ = rotationZ;

	return;
}


void EntityListClass::UpdateRotateState(bool rotate)
{
	m_turningRight = rotate;

	return;
}
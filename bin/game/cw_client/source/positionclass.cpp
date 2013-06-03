////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;

	m_turnLeftStateChange = false;
	m_turningLeft = false;
	m_turnRightStateChange = false;
	m_turningRight = false;
	m_moveForwardStateChange = false;
	m_movingForward = false;
	m_moveBackwardStateChange = false;
	m_movingBackward = false;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		// Convert degrees to radians.
		radians = m_rotationY * 0.0174532925f;

		// Update the position.
		m_positionX += sinf(radians) * 0.3f;
		m_positionZ += cosf(radians) * 0.3f;
	}

	// Update the state.
	if((keydown == true) && (m_movingForward == false))
	{
		m_moveForwardStateChange = true;
		m_movingForward = true;
	}

	if((keydown == false) && (m_movingForward == true))
	{
		m_moveForwardStateChange = true;
		m_movingForward = false;
	}

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		// Convert degrees to radians.
		radians = m_rotationY * 0.0174532925f;

		// Update the position.
		m_positionX -= sinf(radians) * 0.3f;
		m_positionZ -= cosf(radians) * 0.3f;
	}

	// Update the state.
	if((keydown == true) && (m_movingBackward == false))
	{
		m_moveBackwardStateChange = true;
		m_movingBackward = true;
	}

	if((keydown == false) && (m_movingBackward == true))
	{
		m_moveBackwardStateChange = true;
		m_movingBackward = false;
	}

	return;
}


void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		// Update the rotation.
		m_rotationY -= 1.5f;

		// Keep the rotation in the 0 to 360 range.
		if(m_rotationY < 0.0f)
		{
			m_rotationY += 360.0f;
		}
	}

	// Update the state.
	if((keydown == true) && (m_turningLeft == false))
	{
		m_turnLeftStateChange = true;
		m_turningLeft = true;
	}

	if((keydown == false) && (m_turningLeft == true))
	{
		m_turnLeftStateChange = true;
		m_turningLeft = false;
	}

	return;
}


void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		// Update the rotation.
		m_rotationY += 1.5f;

		// Keep the rotation in the 0 to 360 range.
		if(m_rotationY > 360.0f)
		{
			m_rotationY -= 360.0f;
		}
	}

	// Update the state.
	if((keydown == true) && (m_turningRight == false))
	{
		m_turnRightStateChange = true;
		m_turningRight = true;
	}

	if((keydown == false) && (m_turningRight == true))
	{
		m_turnRightStateChange = true;
		m_turningRight = false;
	}

	return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if(m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if(m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}


bool PositionClass::TurnLeftStateChange(char& newState)
{
	bool result;


	// Check if there has been a state change to turning left.
	if(m_turnLeftStateChange)
	{
		result = true;
		m_turnLeftStateChange = false;

		// If there has been check whether it was to start or to stop turning left.
		if(m_turningLeft)
		{
			newState = STATE_TURNING_LEFT;
		}
		else
		{
			newState = STATE_NOT_TURNING_LEFT;
		}
	}
	else
	{
		result = false;
	}

	return result;
}


bool PositionClass::TurnRightStateChange(char& newState)
{
	bool result;


	// Check if there has been a state change to turning right.
	if(m_turnRightStateChange)
	{
		result = true;
		m_turnRightStateChange = false;

		// If there has been check whether it was to start or to stop turning right.
		if(m_turningRight)
		{
			newState = STATE_TURNING_RIGHT;
		}
		else
		{
			newState = STATE_NOT_TURNING_RIGHT;
		}
	}
	else
	{
		result = false;
	}

	return result;
}


bool PositionClass::MoveForwardStateChange(char& newState)
{
	bool result;


	// Check if there has been a state change to moving forward.
	if(m_moveForwardStateChange)
	{
		result = true;
		m_moveForwardStateChange = false;

		// If there has been check whether it was to start or to stop moving forward.
		if(m_movingForward)
		{
			newState = STATE_MOVING_FORWARD;
		}
		else
		{
			newState = STATE_NOT_MOVING_FORWARD;
		}
	}
	else
	{
		result = false;
	}

	return result;
}


bool PositionClass::MoveBackwardStateChange(char& newState)
{
	bool result;


	// Check if there has been a state change to moving backward.
	if(m_moveBackwardStateChange)
	{
		result = true;
		m_moveBackwardStateChange = false;

		// If there has been check whether it was to start or to stop moving backward.
		if(m_movingBackward)
		{
			newState = STATE_MOVING_BACKWARD;
		}
		else
		{
			newState = STATE_NOT_MOVING_BACKWARD;
		}
	}
	else
	{
		result = false;
	}

	return result;
}
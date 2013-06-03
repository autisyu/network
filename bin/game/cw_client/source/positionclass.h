////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


/////////////
// DEFINES //
/////////////
const char STATE_TURNING_LEFT = 0;
const char STATE_NOT_TURNING_LEFT = 1;
const char STATE_TURNING_RIGHT = 2;
const char STATE_NOT_TURNING_RIGHT = 3;
const char STATE_MOVING_FORWARD = 4;
const char STATE_NOT_MOVING_FORWARD = 5;
const char STATE_MOVING_BACKWARD = 6;
const char STATE_NOT_MOVING_BACKWARD = 7;


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

	bool TurnLeftStateChange(char&);
	bool TurnRightStateChange(char&);
	bool MoveForwardStateChange(char&);
	bool MoveBackwardStateChange(char&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;
	float m_lookUpSpeed, m_lookDownSpeed;

	bool m_turnLeftStateChange, m_turningLeft;
	bool m_turnRightStateChange, m_turningRight;
	bool m_moveForwardStateChange, m_movingForward;
	bool m_moveBackwardStateChange, m_movingBackward;
};

#endif
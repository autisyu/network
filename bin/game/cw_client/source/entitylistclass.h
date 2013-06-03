////////////////////////////////////////////////////////////////////////////////
// Filename: entitylistclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENTITYLISTCLASS_H_
#define _ENTITYLISTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


/////////////
// GLOBALS //
/////////////
const int MAX_ENTITIES = 100;
const char ENTITY_TYPE_USER = 0;
const char ENTITY_TYPE_AI = 1;
const char E_STATE_TURNING_LEFT = 0;
const char E_STATE_NOT_TURNING_LEFT = 1;
const char E_STATE_TURNING_RIGHT = 2;
const char E_STATE_NOT_TURNING_RIGHT = 3;
const char E_STATE_MOVING_FORWARD = 4;
const char E_STATE_NOT_MOVING_FORWARD = 5;
const char E_STATE_MOVING_BACKWARD = 6;
const char E_STATE_NOT_MOVING_BACKWARD = 7;


////////////////////////////////////////////////////////////////////////////////
// Class name: EntityListClass
////////////////////////////////////////////////////////////////////////////////
class EntityListClass
{
public:
	EntityListClass();
	EntityListClass(const EntityListClass&);
	~EntityListClass();

	void SetOnlineStatus(bool);
	void SetId(unsigned short);
	void SetType(char);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	bool IsOnline();
	unsigned short GetId();
	char GetEntityType();
	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void UpdateState(char);
	void Frame(float);

	void UpdatePosition(float, float, float, float, float, float);
	void UpdateRotateState(bool);

private:
	bool m_online;
	unsigned short m_entityId;
	char m_entityType;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	bool m_movingForward, m_movingBackward, m_turningLeft, m_turningRight;
};

#endif
////////////////////////////////////////////////////////////////////////////////
// Filename: blackforestclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BLACKFORESTCLASS_H_
#define _BLACKFORESTCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"
#include "positionclass.h"
#include "lightclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
#include "terrainclass.h"
#include "terrainshaderclass.h"
#include "textureshaderclass.h"
#include "modelclass.h"
#include "entitylistclass.h"
#include "frustumclass.h"
#include "quadtreeclass.h"
#include "userinterfaceclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: BlackForestClass
////////////////////////////////////////////////////////////////////////////////
class BlackForestClass
{
public:
	BlackForestClass();
	BlackForestClass(const BlackForestClass&);
	~BlackForestClass();

	bool Initialize(D3DClass*, HWND, int, int, float, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, float, UserInterfaceClass*);

	void AddEntity(unsigned short, char, float, float, float, float, float, float);
	void RemoveEntity(unsigned short);
	bool GetStateChange(char&);
	void UpdateEntityState(unsigned short, char);
	bool PositionUpdate(float&, float&, float&, float&, float&, float&);
	void UpdateEntityPosition(unsigned short, float, float, float, float, float, float);
	void UpdateEntityRotate(unsigned short, bool);

private:
	void HandleMovementInput(InputClass*, float);
	bool Render(D3DClass*, UserInterfaceClass*);

private:
	CameraClass* m_Camera;
	PositionClass* m_Position;
	LightClass* m_Light;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	TerrainClass* m_Terrain;
	TerrainShaderClass* m_TerrainShader;
	TextureShaderClass* m_TextureShader;
	ModelClass *m_CubeModel1, *m_CubeModel2;
	EntityListClass* m_EntityList;
	FrustumClass* m_Frustum;
	float m_screenDepth;
	QuadTreeClass* m_QuadTree;
	
	bool m_stateChange;
	char m_newState;
	unsigned long m_updateTime;
	bool m_positionUpdateReady;
};

#endif
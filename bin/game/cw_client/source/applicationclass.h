////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN  // Use this here to stop <windows.h> from including <winsock.h> which conflicts with <winsock2.h>


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "networkclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "userinterfaceclass.h"
#include "stateclass.h"
#include "blackforestclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	NetworkClass* m_Network;

	TimerClass* m_Timer;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	UserInterfaceClass* m_UserInterface;

	StateClass* m_State;
	BlackForestClass* m_BlackForest;
};

#endif
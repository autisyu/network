////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_D3D = 0;
	m_Network = 0;

	m_Timer = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_UserInterface = 0;

	m_State = 0;
	m_BlackForest = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	
	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if(!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UserInterface->Initialize(m_D3D, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Create the StateClass object.
	m_State = new StateClass;
	if(!m_State)
	{
		return false;
	}

	// Create the black forest object.
	m_BlackForest = new BlackForestClass;
	if(!m_BlackForest)
	{
		return false;
	}

	// Initialize the black forest object.
	result = m_BlackForest->Initialize(m_D3D, hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the black forest object.", L"Error", MB_OK);
		return false;
	}

	// Create the network object.
	m_Network = new NetworkClass;
	if(!m_Network)
	{
		return false;
	}

	// Set the zone and UI pointer before initializing.
	m_Network->SetZonePointer(m_BlackForest);
	m_Network->SetUIPointer(m_UserInterface);

	// Initialize the network object.
	result = m_Network->Initialize("192.168.0.106", 7000);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the network.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the black forest object.
	if(m_BlackForest)
	{
		m_BlackForest->Shutdown();
		delete m_BlackForest;
		m_BlackForest = 0;
	}

	// Release the StateClass object.
	if(m_State)
	{
		delete m_State;
		m_State = 0;
	}

	// Release the user interface object.
	if(m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the network object.
	if(m_Network)
	{
		m_Network->Shutdown();
		delete m_Network;
		m_Network = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;
	char state;
	float posX, posY, posZ, rotX, rotY, rotZ;


	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Do the input frame processing.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Do the network frame processing.
	m_Network->Frame();

	// Do the UI frame processing.
	result = m_UserInterface->Frame(m_D3D, m_Input, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Network->GetLatency());
	if(!result)
	{
		return false;
	}

	// Do the zone frame processing based on the current zone state.
	switch(m_State->GetCurrentState())
	{
		case STATE_BLACKFOREST:
		{
			result = m_BlackForest->Frame(m_D3D, m_Input, m_Timer->GetTime(), m_UserInterface);

			// Check for state changes.
			if(m_BlackForest->GetStateChange(state) == true)
			{
				m_Network->SendStateChange(state);
			}
			
			// Check for position updates.
			if(m_BlackForest->PositionUpdate(posX, posY, posZ, rotX, rotY, rotZ) == true)
			{
				m_Network->SendPositionUpdate(posX, posY, posZ, rotX, rotY, rotZ);
			}

			break;
		}
		default:
		{
			result = true;
			break;
		}
	}

	return result;
}
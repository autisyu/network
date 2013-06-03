////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	int i;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the changes to the keyboard state.
	for(i=0; i<256; i++)
	{
		m_keyChanges[i] = false;
	}
	
	m_newKeyPressed = 0;
	m_keyPressed = false;
	m_shiftDown = false;

	return true;
}


void InputClass::Shutdown()
{
	// Release the mouse.
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}


bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		
	return true;
}


bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if(m_mouseX < 0)  { m_mouseX = 0; }
	if(m_mouseY < 0)  { m_mouseY = 0; }
	
	if(m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	if(m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
	
	// Update the keyboard status with any new key presses.
	UpdateKeys();

	return;
}


bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}


bool InputClass::IsLeftPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsRightPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsPgUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsPgDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_PGDN] & 0x80)
	{
		return true;
	}

	return false;
}


void InputClass::UpdateKeys()
{
	int i;


	// Go through each key.
	for(i=0; i<256; i++)
	{
		// Check if it has been pressed.
		if(m_keyboardState[i] & 0x80)
		{
			// If it has the set it as the new key pressed and update the state.
			if(m_keyChanges[i] == false)
			{
				m_newKeyPressed = i;
				m_keyPressed = true;
				m_keyChanges[i] = true;
			}
		}

		// Otherwise set this key as unchanged.
		else
		{
			m_keyChanges[i] = false;
		}
	}

	// Check to see if either shift key is down.
	if((m_keyboardState[DIK_LSHIFT] & 0x80) || (m_keyboardState[DIK_RSHIFT] & 0x80))
	{
		m_shiftDown = true;
	}
	else
	{
		m_shiftDown = false;
	}

	return;
}


bool InputClass::GetNewKeypress(int& key)
{
	key = 0;

	if(!m_keyPressed)
	{
		return false;
	}

	m_keyPressed = false;

	// Back space.
	if(m_newKeyPressed == DIK_BACK)
	{
		key = 8;
		return true;
	}

	// Enter key.
	if(m_newKeyPressed == DIK_RETURN) 
	{
		key = 13;
		return true;
	}

	// Space bar.
	if(m_newKeyPressed == DIK_SPACE) 
	{
		key = 32;
		return true;
	}

	// F1 key.
	if(m_newKeyPressed == DIK_F1) 
	{
		key = DIK_F1;
		return true;
	}

	// F2 key.
	if(m_newKeyPressed == DIK_F2) 
	{
		key = DIK_F2;
		return true;
	}

	// The keys without the shift key pressed.
	if(!m_shiftDown)
	{
		switch(m_newKeyPressed)
		{
			case DIK_APOSTROPHE: { key = 39;  break; }
			case DIK_COMMA:      { key = 44;  break; }
			case DIK_MINUS:      { key = 45;  break; }
			case DIK_PERIOD:     { key = 46;  break; }
			case DIK_SLASH:      { key = 47;  break; }
			case DIK_0:          { key = 48;  break; }
			case DIK_1:          { key = 49;  break; }
			case DIK_2:          { key = 50;  break; }
			case DIK_3:          { key = 51;  break; }
			case DIK_4:          { key = 52;  break; }
			case DIK_5:          { key = 53;  break; }
			case DIK_6:          { key = 54;  break; }
			case DIK_7:          { key = 55;  break; }
			case DIK_8:          { key = 56;  break; }
			case DIK_9:          { key = 57;  break; }
			case DIK_SEMICOLON:  { key = 59;  break; }
			case DIK_EQUALS:     { key = 61;  break; }		
			case DIK_LBRACKET:   { key = 91;  break; }
			case DIK_BACKSLASH:  { key = 92;  break; }
			case DIK_RBRACKET:   { key = 93;  break; }
			case DIK_GRAVE:      { key = 96;  break; }
			case DIK_A:          { key = 97;  break; }
			case DIK_B:          { key = 98;  break; }
			case DIK_C:          { key = 99;  break; }
			case DIK_D:          { key = 100; break; }
			case DIK_E:          { key = 101; break; }
			case DIK_F:          { key = 102; break; }
			case DIK_G:          { key = 103; break; }
			case DIK_H:          { key = 104; break; }
			case DIK_I:          { key = 105; break; }
			case DIK_J:          { key = 106; break; }
			case DIK_K:          { key = 107; break; }
			case DIK_L:          { key = 108; break; }
			case DIK_M:          { key = 109; break; }
			case DIK_N:          { key = 110; break; }
			case DIK_O:          { key = 111; break; }
			case DIK_P:          { key = 112; break; }
			case DIK_Q:          { key = 113; break; }
			case DIK_R:          { key = 114; break; }
			case DIK_S:          { key = 115; break; }
			case DIK_T:          { key = 116; break; }
			case DIK_U:          { key = 117; break; }
			case DIK_V:          { key = 118; break; }
			case DIK_W:          { key = 119; break; }
			case DIK_X:          { key = 120; break; }
			case DIK_Y:          { key = 121; break; }
			case DIK_Z:          { key = 122; break; }
			default:             { key = 0;   break; }
		}
	}

	// The keys with the shift key pressed.
	if(m_shiftDown)
	{
		switch(m_newKeyPressed)
		{
			case DIK_1:          { key = 33;  break; }
			case DIK_APOSTROPHE: { key = 34;  break; }
			case DIK_3:          { key = 35;  break; }
			case DIK_4:          { key = 36;  break; }
			case DIK_5:          { key = 37;  break; }
			case DIK_7:          { key = 38;  break; }
			case DIK_9:          { key = 40;  break; }
			case DIK_0:          { key = 41;  break; }
			case DIK_8:          { key = 42;  break; }
			case DIK_EQUALS:     { key = 43;  break; }
			case DIK_SEMICOLON:  { key = 58;  break; }
			case DIK_COMMA:      { key = 60;  break; }
			case DIK_PERIOD:     { key = 62;  break; }
			case DIK_SLASH:      { key = 63;  break; }
			case DIK_2:          { key = 64;  break; }
			case DIK_A:          { key = 65;  break; }
			case DIK_B:          { key = 66;  break; }
			case DIK_C:          { key = 67;  break; }
			case DIK_D:          { key = 68;  break; }
			case DIK_E:          { key = 69;  break; }
			case DIK_F:          { key = 70;  break; }
			case DIK_G:          { key = 71;  break; }
			case DIK_H:          { key = 72;  break; }
			case DIK_I:          { key = 73;  break; }
			case DIK_J:          { key = 74;  break; }
			case DIK_K:          { key = 75;  break; }
			case DIK_L:          { key = 76;  break; }
			case DIK_M:          { key = 77;  break; }
			case DIK_N:          { key = 78;  break; }
			case DIK_O:          { key = 79;  break; }
			case DIK_P:          { key = 80;  break; }
			case DIK_Q:          { key = 81;  break; }
			case DIK_R:          { key = 82;  break; }
			case DIK_S:          { key = 83;  break; }
			case DIK_T:          { key = 84;  break; }
			case DIK_U:          { key = 85;  break; }
			case DIK_V:          { key = 86;  break; }
			case DIK_W:          { key = 87;  break; }
			case DIK_X:          { key = 88;  break; }
			case DIK_Y:          { key = 89;  break; }
			case DIK_Z:          { key = 90;  break; }
			case DIK_6:          { key = 94;  break; }
			case DIK_MINUS:      { key = 95;  break; }
			case DIK_LBRACKET:   { key = 123; break; }
			case DIK_BACKSLASH:  { key = 124; break; }
			case DIK_RBRACKET:   { key = 125; break; }
			case DIK_GRAVE:      { key = 126; break; }
			default:             { key = 0;   break; }
		}
	}

	return true;
}
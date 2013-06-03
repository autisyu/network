////////////////////////////////////////////////////////////////////////////////
// Filename: userinterfaceclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "userinterfaceclass.h"


UserInterfaceClass::UserInterfaceClass()
{
	m_ChatWindow = 0;
	m_Text = 0;
}


UserInterfaceClass::UserInterfaceClass(const UserInterfaceClass& other)
{
}


UserInterfaceClass::~UserInterfaceClass()
{
}


bool UserInterfaceClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;


	// Set the location of the chat window based on the resolution the application is running at.
	m_chatWindowX = ((screenWidth / 2) - 300);
	m_chatWindowY = (screenHeight - 215);
	
	// Create the chat window bitmap object.
	m_ChatWindow = new BitmapClass;
	if(!m_ChatWindow)
	{
		return false;
	}

	// Initialize the chat window bitmap object.
	result = m_ChatWindow->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/chat.dds", 600, 200, m_chatWindowX, m_chatWindowY);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the chat window object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(Direct3D->GetDevice(), hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	result = SetupTextStrings(Direct3D);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text strings.", L"Error", MB_OK);
		return false;
	}

	//
	m_talkBarPosition = 0;

	m_chatBarString[0] = '\0';
	m_chatString1[0] = '\0';
	m_chatString2[0] = '\0';
	m_chatString3[0] = '\0';
	m_chatString4[0] = '\0';
	m_chatString5[0] = '\0';
	m_chatString6[0] = '\0';
	m_chatString7[0] = '\0';

	m_newMessage = false;

	return true;
}


void UserInterfaceClass::Shutdown()
{
	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the chat window object.
	if(m_ChatWindow)
	{
		m_ChatWindow->Shutdown();
		delete m_ChatWindow;
		m_ChatWindow = 0;
	}

	return;
}


bool UserInterfaceClass::Frame(D3DClass* Direct3D, InputClass* Input, int fps, int cpu, int latency)
{
	bool result;
	int key;


	// Check if any of the other keys have been pressed.
	result = Input->GetNewKeypress(key);
	if(result)
	{
		KeyPressUpdate(key, Direct3D->GetDevice());
	}

	// Set the frames per second.
	result = UpdateFps(fps);
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = UpdateCpu(cpu);
	if(!result)
	{
		return false;
	}

	// Set the network latency.
	result = UpdateLatency(latency);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::Render(D3DClass* Direct3D, TextureShaderClass* TextureShader, D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;


	// Render the chat window using the texture shader.
	result = m_ChatWindow->Render(Direct3D->GetDevice(), m_ChatWindow->GetLocationX(), m_ChatWindow->GetLocationY());  if(!result) { return false; }
	TextureShader->Render(Direct3D->GetDevice(), m_ChatWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, m_ChatWindow->GetTexture());

	// Render the text strings.
	m_Text->Render(Direct3D->GetDevice(), worldMatrix, baseViewMatrix, orthoMatrix);

	return true;
}


bool UserInterfaceClass::SetupTextStrings(D3DClass* Direct3D)
{
	char videoCard[128];
	int videoMemory;
	bool result;


	// Retrieve the video card information.
	Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = SetVideoCardInfo(Direct3D->GetDevice(), videoCard, videoMemory);
	if(!result)
	{
		return false;
	}

	// Set the initial fps string.
	result = SetFps(Direct3D->GetDevice());
	if(!result)
	{
		return false;
	}

	// Set the initial cpu string.
	result = SetCpu(Direct3D->GetDevice());
	if(!result)
	{
		return false;
	}

	// Set the initial latency string.
	result = SetLatency(Direct3D->GetDevice());
	if(!result)
	{
		return false;
	}

	// Set the initial terrain polygon render count.
	result = SetDrawCount(Direct3D->GetDevice());
	if(!result)
	{
		return false;
	}

	// Set the initial chat window text strings.
	result = SetChatTextStrings(Direct3D->GetDevice());
	if(!result)
	{
		return false;
	}
	
	return true;
}


bool UserInterfaceClass::SetVideoCardInfo(ID3D10Device* device, char* videoCardName, int videoCardMemory)
{
	char dataString[150];
	bool result;
	char tempString[16];
	char memoryString[32];


	// Setup the video card name string.
	strcpy_s(dataString, "Video Card: ");
	strcat_s(dataString, videoCardName);

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 0, 150, dataString, 10, 10, 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	// Truncate the memory value to prevent buffer over flow.
	if(videoCardMemory > 9999999)
	{
		videoCardMemory = 9999999;
	}

	// Convert the video memory integer value to a string format.
	_itoa_s(videoCardMemory, tempString, 10);

	// Setup the video memory string.
	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 1, 32, memoryString, 10, 30, 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::SetFps(ID3D10Device* device)
{
	bool result;

	
	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 2, 16, "Fps: 0", 10, 50, 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::UpdateFps(int fps)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;


	// Truncate the fps to below 100,000.
	if(fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if(fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if(fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if(fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(2, fpsString, 10, 50, red, green, blue);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::SetCpu(ID3D10Device* device)
{
	bool result;

	
	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 3, 16, "Cpu: 0%", 10, 70, 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::UpdateCpu(int cpu)
{
	char tempString[16];
	char cpuString[16];
	bool result;


	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(3, cpuString, 10, 70, 0.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::SetLatency(ID3D10Device* device)
{
	bool result;

	
	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 4, 16, "Latency: 0 ms", 10, 90, 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::UpdateLatency(int latency)
{
	char tempString[16];
	char latencyString[16];
	bool result;


	// Truncate the latency.
	if(latency > 9999)
	{
		latency = 9999;
	}

	// Convert the latency integer to string format.
	_itoa_s(latency, tempString, 10);

	// Setup the latency string.
	strcpy_s(latencyString, "Latency: ");
	strcat_s(latencyString, tempString);
	strcat_s(latencyString, " ms");

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(4, latencyString, 10, 90, 0.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::SetDrawCount(ID3D10Device* device)
{
	bool result;

	
	// Update the sentence vertex buffer with the new string information.
	result = m_Text->CreateSentence(device, 5, 32, "Render count: 0", 10, 110, 0.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::UpdateDrawCount(int drawCount)
{
	char tempString[32];
	char countString[32];
	bool result;


	// Convert the draw count integer to string format.
	_itoa_s(drawCount, tempString, 10);

	// Setup the latency string.
	strcpy_s(countString, "Render count: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(5, countString, 10, 110, 0.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::SetChatTextStrings(ID3D10Device* device)
{
	bool result;


	// Chat string 1.
	result = m_Text->CreateSentence(device, 6, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 20), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 2.
	result = m_Text->CreateSentence(device, 7, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 40), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 3.
	result = m_Text->CreateSentence(device, 8, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 60), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 4.
	result = m_Text->CreateSentence(device, 9, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 80), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 5.
	result = m_Text->CreateSentence(device, 10, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 100), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 6.
	result = m_Text->CreateSentence(device, 11, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 120), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Chat string 7.
	result = m_Text->CreateSentence(device, 12, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 140), 1.0f, 1.0f, 0.0f);
	if(!result)
	{
		return false;
	}

	// Talk bar string.
	result = m_Text->CreateSentence(device, 13, 50, " ", (m_chatWindowX + 20), (m_chatWindowY + 180), 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


void UserInterfaceClass::KeyPressUpdate(int key, ID3D10Device* device)
{
	if(key == 0)
	{
		return;
	}

	// Back space.
	if(key == 8)
	{
		if(m_talkBarPosition > 0)
		{
			m_talkBarPosition--;
			m_chatBarString[m_talkBarPosition] = '\0';
			UpdateChatBar(device);
		}
		return;
	}

	// Enter key.
	if(key == 13)
	{
		AddChatMessage();
		AddChatMessageForServer();
		m_chatBarString[0] = '\0';
		m_talkBarPosition = 0;
		UpdateChatBar(device);
		return;
	}

	if(m_talkBarPosition < 38)
	{
		m_chatBarString[m_talkBarPosition] = key;
		m_talkBarPosition++;
		m_chatBarString[m_talkBarPosition] = '\0';
		UpdateChatBar(device);
	}

	return;
}


bool UserInterfaceClass::UpdateChatBar(ID3D10Device* device)
{
	bool result;


	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(13, m_chatBarString, (m_chatWindowX + 20), (m_chatWindowY + 180), 1.0f, 1.0f, 1.0f);
	if(!result)
	{
		return false;
	}

	return true;
}


bool UserInterfaceClass::AddChatMessage()
{
	bool result;


	strcpy_s(m_chatString1, m_chatString2);
	strcpy_s(m_chatString2, m_chatString3);
	strcpy_s(m_chatString3, m_chatString4);
	strcpy_s(m_chatString4, m_chatString5);
	strcpy_s(m_chatString5, m_chatString6);
	strcpy_s(m_chatString6, m_chatString7);
	strcpy_s(m_chatString7, "You say: ");
	strcat_s(m_chatString7, m_chatBarString);

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(6, m_chatString1, (m_chatWindowX + 20), (m_chatWindowY + 20), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(7, m_chatString2, (m_chatWindowX + 20), (m_chatWindowY + 40), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(8, m_chatString3, (m_chatWindowX + 20), (m_chatWindowY + 60), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(9, m_chatString4, (m_chatWindowX + 20), (m_chatWindowY + 80), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(10, m_chatString5, (m_chatWindowX + 20), (m_chatWindowY + 100), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(11, m_chatString6, (m_chatWindowX + 20), (m_chatWindowY + 120), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(12, m_chatString7, (m_chatWindowX + 20), (m_chatWindowY + 140), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }

	return true;
}


void UserInterfaceClass::AddChatMessageForServer()
{
	m_newMessage = true;
	strcpy_s(m_serverMessage, m_chatBarString);
	return;
}


void UserInterfaceClass::CheckForChatMessage(char* message, bool& newMessage)
{
	if(m_newMessage)
	{
		newMessage = true;
		strcpy_s(message, 50, m_serverMessage);
		m_newMessage = false;
	}
	else
	{
		newMessage = false;
	}

	return;
}


bool UserInterfaceClass::AddChatMessageFromServer(char* message, int id)
{
	char tempString[16];
	char textString[50];
	bool result;


	// Convert the id number to string format.
	_itoa_s(id, tempString, 10);

	strcpy_s(textString, "User ");
	strcat_s(textString, tempString);
	strcat_s(textString, " says: ");
	strcat_s(textString, message);

	strcpy_s(m_chatString1, m_chatString2);
	strcpy_s(m_chatString2, m_chatString3);
	strcpy_s(m_chatString3, m_chatString4);
	strcpy_s(m_chatString4, m_chatString5);
	strcpy_s(m_chatString5, m_chatString6);
	strcpy_s(m_chatString6, m_chatString7);
	strcpy_s(m_chatString7, textString);

	// Update the sentence vertex buffer with the new string information.
	result = m_Text->UpdateSentence(6, m_chatString1, (m_chatWindowX + 20), (m_chatWindowY + 20), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(7, m_chatString2, (m_chatWindowX + 20), (m_chatWindowY + 40), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(8, m_chatString3, (m_chatWindowX + 20), (m_chatWindowY + 60), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(9, m_chatString4, (m_chatWindowX + 20), (m_chatWindowY + 80), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(10, m_chatString5, (m_chatWindowX + 20), (m_chatWindowY + 100), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(11, m_chatString6, (m_chatWindowX + 20), (m_chatWindowY + 120), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }
	result = m_Text->UpdateSentence(12, m_chatString7, (m_chatWindowX + 20), (m_chatWindowY + 140), 1.0f, 1.0f, 0.0f);  if(!result) { return false; }

	return true;
}
////////////////////////////////////////////////////////////////////////////////
// Filename: blackforestclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "blackforestclass.h"


BlackForestClass::BlackForestClass()
{
	m_Camera = 0;
	m_Position = 0;
	m_Light = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_Terrain = 0;
	m_TerrainShader = 0;
	m_TextureShader = 0;
	m_CubeModel1 = 0;
	m_CubeModel2 = 0;
	m_EntityList = 0;
	m_Frustum = 0;
	m_QuadTree = 0;
}


BlackForestClass::BlackForestClass(const BlackForestClass& other)
{
}


BlackForestClass::~BlackForestClass()
{
}


bool BlackForestClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	bool result;


	// Store screen depth for frustum.
	m_screenDepth = screenDepth;

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->GenerateBaseViewMatrix();

	// Initialize the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer.
	m_Position->SetPosition(0.0f, 0.0f, 0.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.75f, 0.25f);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if(!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if(!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize("../Engine/data/blackforest_hm.bmp", "../Engine/data/blackforest_cm.bmp", 12.5f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the slope shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_CubeModel1 = new ModelClass;
	if(!m_CubeModel1)
	{
		return false;
	}

	// Initialize the model object.
	result = m_CubeModel1->Initialize(Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/char1.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_CubeModel2 = new ModelClass;
	if(!m_CubeModel2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_CubeModel2->Initialize(Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/char2.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the secpmd model object.", L"Error", MB_OK);
		return false;
	}

	// Create the entity list array.
	m_EntityList = new EntityListClass[MAX_ENTITIES];
	if(!m_EntityList)
	{
		MessageBox(hwnd, L"Could not initialize the entity list array.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}

	// Create the quad tree object.
	m_QuadTree = new QuadTreeClass;
	if(!m_QuadTree)
	{
		return false;
	}

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	// Release the terrain object now that the quad tree has been loaded.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Set initial value of state change variables for entities.
	m_stateChange = false;
	m_updateTime = timeGetTime();
	m_positionUpdateReady = false;

	return true;
}


void BlackForestClass::Shutdown()
{
	// Release the quad tree object.
	if(m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the entity list array.
	if(m_EntityList)
	{
		delete [] m_EntityList;
		m_EntityList = 0;
	}

	// Release the model objects.
	if(m_CubeModel1)
	{
		m_CubeModel1->Shutdown();
		delete m_CubeModel1;
		m_CubeModel1 = 0;
	}
	if(m_CubeModel2)
	{
		m_CubeModel2->Shutdown();
		delete m_CubeModel2;
		m_CubeModel2 = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the sky dome shader object.
	if(m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if(m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}


bool BlackForestClass::Frame(D3DClass* Direct3D, InputClass* Input, float frameTime, UserInterfaceClass* UserInterface)
{
	bool result;
	int i;


	// Update the movement of the camera location based on the user input.
	HandleMovementInput(Input, frameTime);

	// Do the frame processing for the entity list.
	for(i=0; i<MAX_ENTITIES; i++)
	{
		if(m_EntityList[i].IsOnline())
		{
			m_EntityList[i].Frame(frameTime);
		}
	}

	// If 3 seconds is up then send a message to the server with our updated location.
	if(timeGetTime() >= (m_updateTime + 3000))
	{
		m_updateTime = timeGetTime();
		m_positionUpdateReady = true;
	}

	// Render the graphics.
	result = Render(Direct3D, UserInterface);
	if(!result)
	{
		return false;
	}

	return true;
}


void BlackForestClass::HandleMovementInput(InputClass* Input, float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;
	char state;


	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Left key press.
	keyDown = Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	// Check for a state change of turning left.
	result = m_Position->TurnLeftStateChange(state);
	if(result)
	{
		m_stateChange = true;
		m_newState = state;
	}

	keyDown = Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	// Check for a state change of turning right.
	result = m_Position->TurnRightStateChange(state);
	if(result)
	{
		m_stateChange = true;
		m_newState = state;
	}

	keyDown = Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	// Check for a state change of moving forward.
	result = m_Position->MoveForwardStateChange(state);
	if(result)
	{
		m_stateChange = true;
		m_newState = state;
	}

	keyDown = Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	// Check for a state change of moving backward.
	result = m_Position->MoveBackwardStateChange(state);
	if(result)
	{
		m_stateChange = true;
		m_newState = state;
	}
	
	keyDown = Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	return;
}


bool BlackForestClass::Render(D3DClass* Direct3D, UserInterfaceClass* UserInterface)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix, translateMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result, foundHeight;
	int i;
	float posX, posY, posZ, rotX, rotY, rotZ, height;


	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Get the height of the triangle that is directly underneath the given camera position.
	foundHeight =  m_QuadTree->GetHeightAtPosition(cameraPosition.x, cameraPosition.z, height);
	if(foundHeight)
	{
		// If there was a triangle under the camera then position the camera just above it by two units.
		m_Camera->SetPosition(cameraPosition.x, height + 2.0f, cameraPosition.z);
	}

	// Clear the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Construct the frustum.
	m_Frustum->ConstructFrustum(m_screenDepth, projectionMatrix, viewMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(Direct3D->GetDevice());
	m_SkyDomeShader->Render(Direct3D->GetDevice(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	Direct3D->TurnOnCulling();

	// Turn the Z buffer back on.
	Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain inside the quad tree using the terrain shader.
	m_TerrainShader->SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, m_Light->GetDiffuseColor(), m_Light->GetDirection());
	m_QuadTree->Render(m_Frustum, Direct3D->GetDevice(), m_TerrainShader);
	
	// Update the terrain polygon render count string.
	result = UserInterface->UpdateDrawCount(m_QuadTree->GetDrawCount());
	if(!result)
	{ 
		return false;
	}

	// Render all the online users.
	for(i=0; i<MAX_ENTITIES; i++)
	{
		// Render them only if they are online.
		if(m_EntityList[i].IsOnline())
		{
			// Get the position and rotation of the online user.
			m_EntityList[i].GetPosition(posX, posY, posZ);
			m_EntityList[i].GetRotation(rotX, rotY, rotZ);

			foundHeight =  m_QuadTree->GetHeightAtPosition(posX, posZ, height);
			if(foundHeight)
			{
				posY = height + 1.0f;
			}

			// Transform the world matrix by the user location.
			D3DXMatrixRotationY(&worldMatrix, rotY * ((float)D3DX_PI / 180.0f));
			D3DXMatrixTranslation(&translateMatrix, posX, posY, posZ);
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 

			if(m_EntityList[i].GetEntityType() == ENTITY_TYPE_USER)
			{
				// Render the cube model with the texture shader to represent the online user.
				m_CubeModel1->Render(Direct3D->GetDevice());
				m_TextureShader->Render(Direct3D->GetDevice(), m_CubeModel1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_CubeModel1->GetTexture());
			}

			if(m_EntityList[i].GetEntityType() == ENTITY_TYPE_AI)
			{
				// Render the cube model with the texture shader to represent the AI entity.
				m_CubeModel2->Render(Direct3D->GetDevice());
				m_TextureShader->Render(Direct3D->GetDevice(), m_CubeModel2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_CubeModel2->GetTexture());
			}

			// Reset the world matrix.
			Direct3D->GetWorldMatrix(worldMatrix);
		}
	}

	// Turn off the Z buffer and enable alpha blending to begin all 2D rendering.
	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaBlending();

	// Render the user interface.
	result = UserInterface->Render(Direct3D, m_TextureShader, worldMatrix, baseViewMatrix, orthoMatrix);
	if(!result)
	{ 
		return false;
	}

	// Turn the Z buffer back on and disable alpha blending now that all 2D rendering has completed.
	Direct3D->DisableAlphaBlending();
	Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	Direct3D->EndScene();

	return true;
}


void BlackForestClass::AddEntity(unsigned short entityId, char entityType, float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ)
{
	int i;


	// Loop through the entity list and find a free spot.
	i=0;
	while((m_EntityList[i].IsOnline() == true) && (i < MAX_ENTITIES))
	{
		i++;
	}

	if(i == MAX_ENTITIES)
	{

	}
	else
	{
		// Add the entity to the zone.
		m_EntityList[i].SetOnlineStatus(true);
		m_EntityList[i].SetId(entityId);
		m_EntityList[i].SetType(entityType);
		m_EntityList[i].SetPosition(positionX, positionY, positionZ);
		m_EntityList[i].SetRotation(rotationX, rotationY, rotationZ);
	}
	
	return;
}


void BlackForestClass::RemoveEntity(unsigned short entityId)
{
	int i;
	bool found;

	i=0;
	found = false;
	while(!found)
	{
		if((m_EntityList[i].GetId() == entityId) && (m_EntityList[i].GetEntityType() == ENTITY_TYPE_USER))
		{
			// Remove the entity to the zone.
			m_EntityList[i].SetOnlineStatus(false);
			found = true;
		}
		else
		{
			i++;
		}
	}

	return;
}


bool BlackForestClass::GetStateChange(char& state)
{
	// Determine if there was a state change based on the movement input of the client.
	if(m_stateChange)
	{
		state = m_newState;
		m_stateChange = false;
		return true;
	}
	else
	{
		return false;
	}
}


void BlackForestClass::UpdateEntityState(unsigned short entityId, char state)
{
	int i;
	bool found;


	// Find the entity in the entity list.
	i=0;
	found = false;
	while(!found)
	{
		if((m_EntityList[i].GetId() == entityId) && (m_EntityList[i].GetEntityType() == ENTITY_TYPE_USER))
		{
			found = true;
		}
		else
		{
			i++;
		}
	}

	// Update their current state.
	m_EntityList[i].UpdateState(state);

	return;
}


bool BlackForestClass::PositionUpdate(float& posX, float& posY, float& posZ, float& rotX, float& rotY, float& rotZ)
{
	// Check if it is time to send the server an update of our position in the zone.
	if(m_positionUpdateReady)
	{
		// Get the view point position/rotation.
		m_Position->GetPosition(posX, posY, posZ);
		m_Position->GetRotation(rotX, rotY, rotZ);
		m_positionUpdateReady = false;
		return true;
	}
	else
	{
		return false;
	}
}


void BlackForestClass::UpdateEntityPosition(unsigned short entityId, float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ)
{
	int i;
	bool found;


	// Find the position of the entity in the list.
	i=0;
	found = false;
	while(!found)
	{
		if((m_EntityList[i].GetId() == entityId) && (m_EntityList[i].GetEntityType() == ENTITY_TYPE_USER))
		{
			found = true;
		}
		else
		{
			i++;
		}
	}

	// Update their position in the zone.
	m_EntityList[i].UpdatePosition(positionX, positionY, positionZ, rotationX, rotationY, rotationZ);

	return;
}


void BlackForestClass::UpdateEntityRotate(unsigned short entityId, bool rotate)
{
	int i;
	bool found;


	// Find the position of the AI entity in the list.
	i=0;
	found = false;
	while(!found)
	{
		if((m_EntityList[i].GetId() == entityId) && (m_EntityList[i].GetEntityType() == ENTITY_TYPE_AI))
		{
			found = true;
		}
		else
		{
			i++;
		}
	}

	// Update the AI entity rotation state.
	m_EntityList[i].UpdateRotateState(rotate);

	return;
}
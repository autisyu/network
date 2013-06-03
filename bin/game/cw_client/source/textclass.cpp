///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"


TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;
	m_sentenceArray = 0;
}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	int i;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the font object.
	m_Font = new FontClass;
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../Engine/data/font/fontdata.txt", L"../Engine/data/font/font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the array of sentences.
	m_sentenceArray = new SentenceType[NUM_SENTENCES];
	if(!m_sentenceArray)
	{
		return false;
	}

	// Set all the sentences to not active for now.
	for(i=0; i<NUM_SENTENCES; i++)
	{
		m_sentenceArray[i].active = false;
	}

	return true;
}


void TextClass::Shutdown()
{
	int i;


	// Release the sentences.
	if(m_sentenceArray)
	{
		for(i=0; i<NUM_SENTENCES; i++)
		{
			if(m_sentenceArray[i].active)
			{
				ReleaseSentence(i);
			}
		}

		delete [] m_sentenceArray;
		m_sentenceArray = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


void TextClass::Render(ID3D10Device* device, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	int i;


	// Draw the sentences.
	for(i=0; i<NUM_SENTENCES; i++)
	{
		if(m_sentenceArray[i].active)
		{
			RenderSentence(device, i, worldMatrix, viewMatrix, orthoMatrix);
		}
	}

	return;
}


bool TextClass::CreateSentence(ID3D10Device* device, int id, int maxLength, char* sentence, int posX, int posY, float red, float green, float blue)
{
	bool result;


	// Initialize the sentence.
	result = InitializeSentence(id, maxLength, device);
	if(!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(id, sentence, posX, posY, red, green, blue);
	if(!result)
	{
		return false;
	}

	return true;
}


bool TextClass::InitializeSentence(int sentenceId, int maxLength, ID3D10Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Set the sentence to active.
	m_sentenceArray[sentenceId].active = true;

	// Initialize the sentence buffers to null.
	m_sentenceArray[sentenceId].vertexBuffer = 0;
	m_sentenceArray[sentenceId].indexBuffer = 0;

	// Set the maximum length of the sentence.
	m_sentenceArray[sentenceId].maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	m_sentenceArray[sentenceId].vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	m_sentenceArray[sentenceId].indexCount = m_sentenceArray[sentenceId].vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_sentenceArray[sentenceId].vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_sentenceArray[sentenceId].indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_sentenceArray[sentenceId].vertexCount));

	// Initialize the index array.
	for(i=0; i<m_sentenceArray[sentenceId].indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_sentenceArray[sentenceId].vertexCount;
    vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_sentenceArray[sentenceId].vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_sentenceArray[sentenceId].indexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_sentenceArray[sentenceId].indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(int sentenceId, char* text, int positionX, int positionY, float red, float green, float blue)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	void* verticesPtr;
	HRESULT result;


	// Store the color of the sentence.
	m_sentenceArray[sentenceId].red = red;
	m_sentenceArray[sentenceId].green = green;
	m_sentenceArray[sentenceId].blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > m_sentenceArray[sentenceId].maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[m_sentenceArray[sentenceId].vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_sentenceArray[sentenceId].vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Initialize the vertex buffer pointer to null first.
	verticesPtr = 0;

	// Lock the vertex buffer.
	result = m_sentenceArray[sentenceId].vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the vertex array into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_sentenceArray[sentenceId].vertexCount));

	// Unlock the vertex buffer.
	m_sentenceArray[sentenceId].vertexBuffer->Unmap();

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(int sentenceId)
{
	// Release the sentence vertex buffer.
	if(m_sentenceArray[sentenceId].vertexBuffer)
	{
		m_sentenceArray[sentenceId].vertexBuffer->Release();
		m_sentenceArray[sentenceId].vertexBuffer = 0;
	}

	// Release the sentence index buffer.
	if(m_sentenceArray[sentenceId].indexBuffer)
	{
		m_sentenceArray[sentenceId].indexBuffer->Release();
		m_sentenceArray[sentenceId].indexBuffer = 0;
	}

	return;
}


void TextClass::RenderSentence(ID3D10Device* device, int sentenceId, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &m_sentenceArray[sentenceId].vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    device->IASetIndexBuffer(m_sentenceArray[sentenceId].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(m_sentenceArray[sentenceId].red, m_sentenceArray[sentenceId].green, m_sentenceArray[sentenceId].blue, 1.0f);

	// Render the text using the font shader.
	m_FontShader->Render(device, m_sentenceArray[sentenceId].indexCount, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);

	return;
}
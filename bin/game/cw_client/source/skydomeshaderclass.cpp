////////////////////////////////////////////////////////////////////////////////
// Filename: skydomeshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skydomeshaderclass.h"


SkyDomeShaderClass::SkyDomeShaderClass()
{
	m_effect = 0;
	m_technique = 0;
	m_layout = 0;

	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;

	m_apexColorPtr = 0;
	m_centerColorPtr = 0;
}


SkyDomeShaderClass::SkyDomeShaderClass(const SkyDomeShaderClass& other)
{
}


SkyDomeShaderClass::~SkyDomeShaderClass()
{
}


bool SkyDomeShaderClass::Initialize(ID3D10Device* device, HWND hwnd)
{
	bool result;


	// Initialize the shader that will be used to draw the triangles.
	result = InitializeShader(device, hwnd, L"../Engine/skydome.fx");
	if(!result)
	{
		return false;
	}

	return true;
}


void SkyDomeShaderClass::Shutdown()
{
	// Shutdown the shader effect.
	ShutdownShader();

	return;
}


void SkyDomeShaderClass::Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
								D3DXVECTOR4 apexColor, D3DXVECTOR4 centerColor)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);

	// Now render the prepared buffers with the shader.
	RenderShader(device, indexCount);

	return;
}


bool SkyDomeShaderClass::InitializeShader(ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
    D3D10_PASS_DESC passDesc;


	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, device, NULL, NULL, &m_effect, &errorMessage, 
										NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Get a pointer to the technique inside the shader.
	m_technique = m_effect->GetTechniqueByName("SkyDomeTechnique");
	if(!m_technique)
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Get the description of the first pass described in the shader technique.
    m_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Get pointers to the three matrices inside the shader so we can update them from this class.
    m_worldMatrixPtr = m_effect->GetVariableByName("worldMatrix")->AsMatrix();
	m_viewMatrixPtr = m_effect->GetVariableByName("viewMatrix")->AsMatrix();
    m_projectionMatrixPtr = m_effect->GetVariableByName("projectionMatrix")->AsMatrix();

	//
	m_apexColorPtr = m_effect->GetVariableByName("apexColor")->AsVector();
	m_centerColorPtr = m_effect->GetVariableByName("centerColor")->AsVector();

	return true;
}


void SkyDomeShaderClass::ShutdownShader()
{
	//
	m_apexColorPtr = 0;
	m_centerColorPtr = 0;

	// Release the pointers to the matrices inside the shader.
	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;

	// Release the pointer to the shader layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pointer to the shader technique.
	m_technique = 0;

	// Release the pointer to the shader.
	if(m_effect)
	{
		m_effect->Release();
		m_effect = 0;
	}

	return;
}


void SkyDomeShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


void SkyDomeShaderClass::SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 apexColor, 
											 D3DXVECTOR4 centerColor)
{
	// Set the world matrix variable inside the shader.
    m_worldMatrixPtr->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	m_viewMatrixPtr->SetMatrix((float*)&viewMatrix);

	// Set the projection matrix variable inside the shader.
    m_projectionMatrixPtr->SetMatrix((float*)&projectionMatrix);

	//
	m_apexColorPtr->SetFloatVector((float*)&apexColor);
	m_centerColorPtr->SetFloatVector((float*)&centerColor);

	return;
}


void SkyDomeShaderClass::RenderShader(ID3D10Device* device, int indexCount)
{
    D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(m_layout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
    m_technique->GetDesc(&techniqueDesc);

    // Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
    {
        m_technique->GetPassByIndex(i)->Apply(0);
        device->DrawIndexed(indexCount, 0, 0);
    }

	return;
}
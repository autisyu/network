////////////////////////////////////////////////////////////////////////////////
// Filename: skydomeshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYDOMESHADERCLASS_H_
#define _SKYDOMESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyDomeShaderClass
////////////////////////////////////////////////////////////////////////////////
class SkyDomeShaderClass
{
public:
	SkyDomeShaderClass();
	SkyDomeShaderClass(const SkyDomeShaderClass&);
	~SkyDomeShaderClass();

	bool Initialize(ID3D10Device*, HWND);
	void Shutdown();
	void Render(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4);

private:
	bool InitializeShader(ID3D10Device*, HWND, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	void SetShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4);
	void RenderShader(ID3D10Device*, int);

private:
	ID3D10Effect* m_effect;
	ID3D10EffectTechnique* m_technique;
	ID3D10InputLayout* m_layout;

	ID3D10EffectMatrixVariable* m_worldMatrixPtr;
	ID3D10EffectMatrixVariable* m_viewMatrixPtr;
	ID3D10EffectMatrixVariable* m_projectionMatrixPtr;

	ID3D10EffectVectorVariable* m_apexColorPtr;
	ID3D10EffectVectorVariable* m_centerColorPtr;
};

#endif
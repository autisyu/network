////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_


/////////////
// GLOBALS //
/////////////
const int NUM_SENTENCES = 20;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass
{
private:
	struct SentenceType
	{
		ID3D10Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D10Device*, HWND, int, int);
	void Shutdown();
	void Render(ID3D10Device*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	bool CreateSentence(ID3D10Device*, int, int, char*, int, int, float, float, float);
	bool UpdateSentence(int, char*, int, int, float, float, float);

private:
	bool InitializeSentence(int, int, ID3D10Device*);
	void ReleaseSentence(int);
	void RenderSentence(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	SentenceType* m_sentenceArray;
};

#endif
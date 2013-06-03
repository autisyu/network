////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10math.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};
	
public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(char*, char*, float);
	void Shutdown();

	int GetVertexCount();
	void CopyVertexArray(void*);

private:
	bool LoadHeightMap(char*);
	bool LoadColorMap(char*);
	void ReduceHeightMap(float);
	bool CalculateNormals();
	bool InitializeVertexArray();

	void ReleaseHeightMap();
	void ReleaseVertexArray();

private:
	int m_terrainWidth, m_terrainHeight, m_vertexCount;
	HeightMapType* m_heightMap;
	VertexType* m_vertexArray;
};

#endif
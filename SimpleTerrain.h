///============================================================================
///@file	SimpleTerrain.h
///@brief	Defines a simple terrain rendering class.
///
///@author	VerMan
///@date	March 7, 2009
///============================================================================

#pragma once

#include <iostream>
#include <fstream>
#include "DXApp.h"
#include "Timer.h"

template <typename T> inline void SafeRelease(T& x)
{
	if(x)
	{
		x->Release();
		x = NULL;
	}
}

//-------------------------------------------------------------------------
//Defines our custom vertex 3D structure
//-------------------------------------------------------------------------
class Vertex3D
{
public:
	Vertex3D(float fx, float fy, float fz, DWORD diffuse = 0xFF000000)
	{x=fx; y=fy; z=fz; color=diffuse;}

	Vertex3D()
	{x=0.0f; y=0.0f; z=0.0f; color=0xFF000000;}

	float x,y,z;
	DWORD color;
};

class SimpleTerrain : public DXApp
{
public:
	//-------------------------------------------------------------------------
	//Constructors and destructors
	//-------------------------------------------------------------------------
	SimpleTerrain();
	~SimpleTerrain();

	//-------------------------------------------------------------------------
	//Public methods
	//-------------------------------------------------------------------------
	virtual void InitData();
	virtual void Render();
	virtual bool ShutDown();
	void LoadHeightMap(const char* filename);
	void CreateTerrain();

	//-------------------------------------------------------------------------
	//Public members
	//-------------------------------------------------------------------------
	static const unsigned int TERRAIN_WIDTH  = 64;	///> Height map width
	static const unsigned int TERRAIN_HEIGHT = 64;	///> Height map height

private:
	//-------------------------------------------------------------------------
	//Private members
	//-------------------------------------------------------------------------
	Timer m_Timer;	///> GL Application timer
	LPTSTR m_FPS;	///> FPS information string
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	DWORD m_VertexCount;
	DWORD m_PrimitiveCount;
	char *m_DeviceDesc;
	unsigned char m_HeightMap[TERRAIN_WIDTH + 1][TERRAIN_HEIGHT + 1];
};


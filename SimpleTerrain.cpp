///============================================================================
///@file	SimpleTerrain.cpp
///@brief	Defines a simple terrain rendering class.
///
///@author	VerMan
///@date	March 7, 2009
///============================================================================

#include "SimpleTerrain.h"

///----------------------------------------------------------------------------
///Default constructor
///----------------------------------------------------------------------------
SimpleTerrain::SimpleTerrain()
{
	DXApp::InitApp("Simple Terrain Rendering", 800, 600);
	DXApp::SetCameraPos(D3DXVECTOR3(0.0f, 50.0f, 90.0f));

	m_FPS = new TCHAR[10];
	m_VertexBuffer = NULL;
	m_IndexBuffer = NULL;
	m_VertexCount = (TERRAIN_WIDTH+1) * (TERRAIN_HEIGHT+1);
	m_PrimitiveCount = TERRAIN_WIDTH * TERRAIN_HEIGHT * 2;
}

///----------------------------------------------------------------------------
///Default destructor
///----------------------------------------------------------------------------
SimpleTerrain::~SimpleTerrain()
{
	//perform cleanup
	ShutDown();
}

///----------------------------------------------------------------------------
///Loads and initializes the terrain vertex & index buffers
///----------------------------------------------------------------------------
void SimpleTerrain::InitData()
{
	LoadHeightMap("heightmap.raw");
	CreateTerrain();
}

///----------------------------------------------------------------------------
///Perform cleanup, unload mesh object
///----------------------------------------------------------------------------
bool SimpleTerrain::ShutDown()
{
	if(m_FPS)
	{
		delete[] m_FPS;
		m_FPS = NULL;
	}

	if(m_DeviceDesc)
	{
		delete m_DeviceDesc;
		m_DeviceDesc = NULL;
	}

	if(m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}

	if(m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}

	return true;
}

///----------------------------------------------------------------------------
///Loads the heightmap from file
///@param	filename - name of the map to load (.raw)
///----------------------------------------------------------------------------
void SimpleTerrain::LoadHeightMap(const char* filename)
{
	std::ifstream File(filename, std::ios::binary);

	for(unsigned y=0; y<(TERRAIN_HEIGHT+1); y++)
	{
		for(unsigned x=0; x<(TERRAIN_WIDTH+1); x++)
		{
			m_HeightMap[x][y] = File.get();
		}
	}

	File.close();
}

///----------------------------------------------------------------------------
///Creates the terrain mesh
///----------------------------------------------------------------------------
void SimpleTerrain::CreateTerrain()
{
	//creates our vertex buffer
	DXApp::GetDevice()->CreateVertexBuffer( sizeof(Vertex3D)*m_VertexCount,
											D3DUSAGE_WRITEONLY,
											D3DFVF_XYZ | D3DFVF_DIFFUSE,
											D3DPOOL_MANAGED,
											&m_VertexBuffer,
											NULL);

	Vertex3D *pVertexData = NULL;
	m_VertexBuffer->Lock(0,0,(void **)&pVertexData,0);
	for(unsigned z=0; z<TERRAIN_HEIGHT+1; z++)
	{
		for(unsigned x=0; x<TERRAIN_WIDTH+1; x++)
		{
			pVertexData[x + z * (TERRAIN_WIDTH+1)].x = (float)x;
			pVertexData[x + z * (TERRAIN_WIDTH+1)].y = (float)m_HeightMap[x][z]/10.0f;
			pVertexData[x + z * (TERRAIN_WIDTH+1)].z = (float)z;
			pVertexData[x + z * (TERRAIN_WIDTH+1)].color = RGB(m_HeightMap[x][z],m_HeightMap[x][z],m_HeightMap[x][z]);
		}
	}
	m_VertexBuffer->Unlock();

	//creates our index buffer
	DXApp::GetDevice()->CreateIndexBuffer(	sizeof(short)*m_PrimitiveCount*3,
											D3DUSAGE_WRITEONLY,
											D3DFMT_INDEX16,
											D3DPOOL_MANAGED,
											&m_IndexBuffer,
											NULL);

	short *pIndexData = NULL;
	m_IndexBuffer->Lock(0,0,(void **)&pIndexData,0);
	for(unsigned z=0; z<TERRAIN_HEIGHT; z++)
	{
		for(unsigned x=0; x<TERRAIN_WIDTH; x++)
		{
			*pIndexData++ = x + z * (TERRAIN_WIDTH + 1); //v1
			*pIndexData++ = x + 1 + z * (TERRAIN_WIDTH + 1); //v2
			*pIndexData++ = x + 1 + (z + 1) * (TERRAIN_WIDTH + 1); //v4
			
			*pIndexData++ = x + z * (TERRAIN_WIDTH + 1); //v1
			*pIndexData++ = x + 1 + (z + 1) * (TERRAIN_WIDTH + 1); //v4
			*pIndexData++ = x + (z + 1) * (TERRAIN_WIDTH + 1); //v3
		}
	}
	m_IndexBuffer->Unlock();
}

///----------------------------------------------------------------------------
///Render the mesh object
///----------------------------------------------------------------------------
void SimpleTerrain::Render()
{
	//lock timer to 60 fps
	m_Timer.Tick(/*60*/);

	//clear buffers
	DXApp::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0, 0);
	
	DXApp::GetDevice()->BeginScene();
	{
		//render some info about our graphics device
		m_Timer.GetFrameRate(m_FPS);
		m_DeviceDesc = strcat(GetAdapterIdentifier().Description,"\n");
		strcat(m_DeviceDesc, m_FPS);
		DXApp::RenderText(m_DeviceDesc);
		DXApp::GetDevice()->SetStreamSource(0,m_VertexBuffer,0,sizeof(Vertex3D));
		DXApp::GetDevice()->SetIndices(m_IndexBuffer);
		DXApp::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_VertexCount,0,m_PrimitiveCount);
	}
	DXApp::GetDevice()->EndScene();

	//swap buffers
	DXApp::GetDevice()->Present(NULL, NULL, NULL, NULL);
}

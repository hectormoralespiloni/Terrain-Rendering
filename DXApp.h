///============================================================================
///@file	DXApp.h
///@brief	Defines the main DirectX application class
///			This class inherits from Application abstract class which
///			takes care of the windows creation and main render loop
///
///@author	Héctor Morales Piloni
///@date	January 15, 2007
///============================================================================

#pragma once

#include <D3DX9.h>
#include <stdio.h>

#include "GraphicsApp.h"

class DXApp : public GraphicsApp
{
public:
	DXApp();
	~DXApp();

	//-------------------------------------------------------------------------
	//Public methods
	//-------------------------------------------------------------------------
	virtual void InitGraphics();
	virtual void RenderText(LPTSTR text);
	virtual void RenderText(LPTSTR text, RECT rc, D3DCOLOR color);
	virtual bool ShutDown();
	virtual LRESULT DisplayWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	virtual void Reshape(int w,int h);
	virtual void Zoom(float zoomFactor);

	void InitApp(LPSTR title, USHORT width, USHORT height);
	void SetCameraPos(D3DXVECTOR3 &cam);
	D3DXVECTOR3 GetCameraPos();
	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXFONT GetFont();
	D3DADAPTER_IDENTIFIER9 GetAdapterIdentifier();

private:
	//-------------------------------------------------------------------------
	//Private methods
	//-------------------------------------------------------------------------
	bool InitDirect3D();
	D3DFORMAT FindDepthStencilFormat(ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType);


	//-------------------------------------------------------------------------
	//Private members
	//-------------------------------------------------------------------------
	LPDIRECT3D9				m_D3D;				///> Direct3D Object
	LPDIRECT3DDEVICE9		m_D3DDevice;		///> Direct3D Device Object
	LPD3DXFONT				m_D3DFont;			///> Direct3D Font Object
	D3DPRESENT_PARAMETERS	m_D3DPresentParams;	///> Direct3D Present Params
	D3DADAPTER_IDENTIFIER9  m_D3DAdapter;		///> Direct3D Adapter Information
	D3DXMATRIX				m_CameraProjMat;	///> Projection matrix
	D3DXMATRIX				m_CameraViewMat;	///> View matrix
	D3DXMATRIX				m_WorldMat;			///> World matrix
	D3DXVECTOR3				m_CameraPos;		///> Camera position
};

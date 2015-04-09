///============================================================================
///@file	DXApp.cpp
///@brief	DirectX Application Class Implementation
///
///@author	Héctor Morales Piloni
///@date	January 15, 2007
///============================================================================

#include "DXApp.h"

///----------------------------------------------------------------------------
///Default constructor.
///----------------------------------------------------------------------------
DXApp::DXApp()
{
	//clear all required values
	m_hWnd	= NULL;
	m_hDC	= NULL;

	//set all required values
	m_WindowTitle	= "DXApp";
	m_Width			= 640;
	m_Height		= 480;
}

///----------------------------------------------------------------------------
///Default destructor.
///----------------------------------------------------------------------------
DXApp::~DXApp()
{
	//perform clean-up
	ShutDown();
}

///----------------------------------------------------------------------------
///Clean up resources.
///----------------------------------------------------------------------------
bool DXApp::ShutDown()
{
	if(m_D3DFont)
	{
		m_D3DFont->Release();
		m_D3DFont = NULL;
	}

	if(m_D3DDevice)
	{
		m_D3DDevice->Release();
		m_D3DDevice = NULL;
	}

	return true;
}

///----------------------------------------------------------------------------
///Initializes the DX Application.
///@param	title - the app window title
///@param	width - the app window width in pixels
///@param	height - the app window height in pixels
///----------------------------------------------------------------------------
void DXApp::InitApp(LPTSTR title, USHORT width, USHORT height)
{
	m_WindowTitle = title;
	m_Width = width;
	m_Height = height;
}

///----------------------------------------------------------------------------
///This function handles messages for the GLApp object
///@param	hWnd - handle to window
///@param	Msg - the message sent
///@param	wParam - message parameter
///@param	lParam - message parameter
///----------------------------------------------------------------------------
LRESULT DXApp::DisplayWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_CREATE:
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			break;

		case WM_CHAR:
			break;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}

///----------------------------------------------------------------------------
///FindDepthStencilFormat ()
///This function simply determines the best depth format that is
///available for the specified mode.
///----------------------------------------------------------------------------
D3DFORMAT DXApp::FindDepthStencilFormat(ULONG AdapterOrdinal, D3DDISPLAYMODE Mode, D3DDEVTYPE DevType)
{
    //Test for 24 bith depth buffer
    if(SUCCEEDED(m_D3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D32)))
    {
        if(SUCCEEDED(m_D3D->CheckDepthStencilMatch (AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D32))) 
		return D3DFMT_D32;
    }

    //Test for 24 bit depth buffer
    if(SUCCEEDED(m_D3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D24X8)))
    {
        if(SUCCEEDED(m_D3D->CheckDepthStencilMatch (AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D24X8))) 
		return D3DFMT_D24X8;
	}

    //Test for 16 bit depth buffer
    if(SUCCEEDED(m_D3D->CheckDeviceFormat(AdapterOrdinal, DevType, Mode.Format, D3DUSAGE_DEPTHSTENCIL , D3DRTYPE_SURFACE , D3DFMT_D16)))
    {
        if(SUCCEEDED(m_D3D->CheckDepthStencilMatch (AdapterOrdinal, DevType, Mode.Format, Mode.Format, D3DFMT_D16))) 
			return D3DFMT_D16;    
    }

    //No depth buffer supported
    return D3DFMT_UNKNOWN;
}

///----------------------------------------------------------------------------
///Initialize Direct3D
///----------------------------------------------------------------------------
bool DXApp::InitDirect3D()
{
	HRESULT					hRet;
	D3DPRESENT_PARAMETERS	presentParams;
	D3DCAPS9				caps;
	D3DDISPLAYMODE			currentMode;

	//create our D3D object
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_D3D) return false;

	//fill out a simple set of present parameters
	ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));

	//set default presentation parameters
	m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentMode);
	presentParams.BackBufferFormat		 = currentMode.Format;
	presentParams.PresentationInterval	 = D3DPRESENT_INTERVAL_IMMEDIATE;
	presentParams.SwapEffect			 = D3DSWAPEFFECT_DISCARD;
	presentParams.AutoDepthStencilFormat = FindDepthStencilFormat(D3DADAPTER_DEFAULT, currentMode, D3DDEVTYPE_HAL);
	presentParams.EnableAutoDepthStencil = TRUE;
	presentParams.Windowed				 = TRUE;
	presentParams.Flags					 = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	//set creation flags
	unsigned long flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//check if HW T&L is available
	ZeroMemory(&caps, sizeof(D3DCAPS9));
	m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		if(caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
			flags |= D3DCREATE_PUREDEVICE;
	}

	//attempt to create a HAL device
	if(FAILED(hRet = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, flags, &presentParams, &m_D3DDevice)))
	{
		//No HAL device, find REF buffer format
		presentParams.AutoDepthStencilFormat = FindDepthStencilFormat(D3DADAPTER_DEFAULT, currentMode, D3DDEVTYPE_REF);

		//check if HW T&L is available
		ZeroMemory(&caps, sizeof(D3DCAPS9));
		flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &caps);

		if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		//attempt to create a REF device
		if(FAILED(hRet = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd, flags, &presentParams, &m_D3DDevice)))
		{
			return false;
		}//failure REF

	}//failure HAL

	//store present params
	m_D3DPresentParams = presentParams;

	//create a font for message display
	D3DXCreateFont(m_D3DDevice, 16, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, 
				   OUT_TT_ONLY_PRECIS, 0, 0, "Verdana", &m_D3DFont);

	//get adapter identifier information
	m_D3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &m_D3DAdapter);

	//success!
	return true;
}

///----------------------------------------------------------------------------
///Initialize DirectX graphic device context.
///----------------------------------------------------------------------------
void DXApp::InitGraphics()
{
	//initialize Direct3D
	if(!InitDirect3D())
	{
		MessageBox(NULL, "Direct3D Initialization failed!", "ERROR", MB_ICONERROR);
		return;
	}

	D3DXMatrixPerspectiveFovLH(&m_CameraProjMat, D3DXToRadian(45.0f), (float)m_Width/(float)m_Height, 1.0f, 1000.0f);
	D3DXMatrixTranslation(&m_WorldMat, -64/2, 0.0f, 0.0f);
	D3DXMatrixLookAtLH( &m_CameraViewMat, 
						&m_CameraPos,						//eye
						&D3DXVECTOR3(0.0f, 0.0f, 0.0f),		//at
						&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	//up

	//setup our D3D Device initial states
	m_D3DDevice->SetTransform(D3DTS_PROJECTION, &m_CameraProjMat);
	m_D3DDevice->SetTransform(D3DTS_VIEW, &m_CameraViewMat);
	m_D3DDevice->SetTransform(D3DTS_WORLD, &m_WorldMat);
	m_D3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    m_D3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_D3DDevice->SetRenderState(D3DRS_AMBIENT, RGB(255,255,255));
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	m_D3DDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
}

///----------------------------------------------------------------------------
///Draws some text in the scene (i.e. FPS, etc)
///Default call
///----------------------------------------------------------------------------
void DXApp::RenderText(LPTSTR text)
{
	RECT rc = {5, 5, 0, 0};
	RenderText(text, rc, D3DCOLOR_ARGB(200,255,255,255));
}

///----------------------------------------------------------------------------
///Draws some text in the scene (i.e. FPS, etc)
///@param	text - text to draw
///@param	rc - rect where the text is being displayed
///@param	color - D3DCOLOR of the text
///----------------------------------------------------------------------------
void DXApp::RenderText(LPTSTR text, RECT rc, D3DCOLOR color)
{
	m_D3DFont->DrawTextA(NULL, text, -1, &rc, DT_NOCLIP, color);
}

///----------------------------------------------------------------------------
///Reset the viewport when window size changes
///@param	w - window width
///@param	h - window height
///----------------------------------------------------------------------------
void DXApp::Reshape(int w, int h)
{

}

///----------------------------------------------------------------------------
///Zoom in/out the camera
///@param	zoomFactor - how much zoom are we going to apply
///			if positive the camera gets closer to the scene,
///			if negative the camera gets further.
///----------------------------------------------------------------------------
void DXApp::Zoom(float zoomFactor)
{

}

///----------------------------------------------------------------------------
///Returns a valid Direct3D rendering device
///----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 DXApp::GetDevice()
{
	if(!m_D3DDevice)
		return NULL;

	return m_D3DDevice;
}

///----------------------------------------------------------------------------
///Returns a D3DFont to draw text
///----------------------------------------------------------------------------
LPD3DXFONT DXApp::GetFont()
{
	if(!m_D3DFont)
		return NULL;

	return m_D3DFont;
}

///----------------------------------------------------------------------------
///Returns a D3DAdapter structure
///----------------------------------------------------------------------------
D3DADAPTER_IDENTIFIER9 DXApp::GetAdapterIdentifier()
{
	return m_D3DAdapter;
}

///----------------------------------------------------------------------------
///Sets the camera position
///@param	cam - D3DVECTOR3 representing the position of the camera
///----------------------------------------------------------------------------
void DXApp::SetCameraPos(D3DXVECTOR3 &cam)
{
	m_CameraPos.x = cam.x;
	m_CameraPos.y = cam.y;
	m_CameraPos.z = cam.z;
}

///----------------------------------------------------------------------------
///Returns the camera position as a D3DVECTOR3
///----------------------------------------------------------------------------
D3DXVECTOR3 DXApp::GetCameraPos()
{
	return m_CameraPos;
}

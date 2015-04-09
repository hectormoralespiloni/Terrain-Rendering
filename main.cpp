///============================================================================
///@file	main.cpp
///@brief	Shadows Mapping Demo.
///
///@author	Héctor Morales Piloni
///@date	January 15, 2007
///============================================================================

#include <windows.h>
#include "SimpleTerrain.h"

SimpleTerrain *myApp;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow)
{
	int retCode;

	//create a new 800x600 window application
	myApp = new SimpleTerrain();
	
	//initilize the application
	if(!myApp->InitInstance(hInstance, lpCmdLine, iCmdShow)) 
	{
		delete myApp;
		return 0;
	}

	//start the application
	retCode = myApp->StartApp();

	//clean-up
	delete myApp;

	return retCode;
}
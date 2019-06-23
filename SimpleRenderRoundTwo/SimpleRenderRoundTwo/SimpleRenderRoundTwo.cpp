using namespace std;

//Hide a console window
#pragma comment(linker, "/subsystem:windows")

//Utilize windows commands
#include <windows.h>
//In/out (printing) functionality
#include <iostream>

//Data structure more similar to js arrays
#include <vector>
//Needed for trig functions
#include <cmath>

//Used for debugging
#include <sstream>
//Access types of variables
#include <typeinfo>

#define IDT_DRAWTIMER 500;

float yDeltaAngle = 0.5;


//Function to draw everything in the new window
void TestPaint(HDC hDC, HWND hwnd, boolean redraw)
{

	const int numPoints = 8;
	float cameraX = 200;
	float cameraY = 200;
	float cameraZ = 200;

	//float currentAngle = xAngle;

	struct drawPoint {
		float xValue;
		float yValue;
		float zValue;
		vector<int> endPoints;
	} allPoints[numPoints];

	float centerPoint[3] = { 200, 200, 200 };
	//All relative to centerPoint
	float xValues[numPoints] = { 100, -100, -100, 100, -100, 100, 100, -100 };
	float yValues[numPoints] = { -100, -100, 100, 100, -100, -100, 100, 100 };
	float zValues[numPoints] = { -100, -100, -100, -100, 100, 100, 100, 100 };
	//int linesToDraw = new int[3];
	//string linesToDraw[numPoints] = { "1,3","2","","" };

	//Assign values to the drawPoint structs

	int i;
	int j;
	for (i = 0; i < numPoints; i++) {
		//OutputDebugStringA(" \n LOOPA   ");
		////Calculate effects of rotation, relative to centerPoint
		//Get hypotenuse lengths
		float XZLength = sqrt(pow(xValues[i], 2) + pow(zValues[i], 2));

		//Find initial angle
		float yInitAngle = acos(xValues[i] / XZLength);
		if (zValues[i] < 0) {
			yInitAngle += 3.14159265358979323846;
		}
		ostringstream newStringAngle;
		newStringAngle << yInitAngle;
		string sAngle(newStringAngle.str());
		//OutputDebugStringA(sAngle.c_str());
		//OutputDebugStringA("  ");

		//Find x, z values
		float rotatedX = XZLength * cos(yInitAngle + yDeltaAngle);
		float rotatedZ = XZLength * sin(yInitAngle + yDeltaAngle);

		////Translate based on centerpoint
		float translatedX = rotatedX + centerPoint[0];
		float translatedY = yValues[i] + centerPoint[1];
		float translatedZ = rotatedZ + centerPoint[2];

		ostringstream newStringZ;
		newStringZ << translatedZ;
		string sZ(newStringZ.str());
		//OutputDebugStringA(sZ.c_str());

		////Calculate draw locations based on perspective
		allPoints[i].xValue = cameraX + ((translatedX - cameraX) * cameraZ) / (cameraZ + translatedZ);
		allPoints[i].yValue = cameraY + ((translatedY - cameraY) * cameraZ) / (cameraZ + translatedZ);

		ostringstream newStringX;
		newStringX << allPoints[i].xValue;
		string sx(newStringX.str());
		//OutputDebugStringA(sx.c_str());

		ostringstream newStringY;
		newStringY << allPoints[i].yValue;
		string sy(newStringY.str());
		//OutputDebugStringA(sy.c_str());

		//Assign lineNodes - do this once
		if (i == 0) {
			allPoints[i].endPoints.resize(3);
			allPoints[i].endPoints[0] = { 1 };
			allPoints[i].endPoints[1] = { 3 };
			allPoints[i].endPoints[2] = { 4 };
		} else if (i == 1) {
			allPoints[i].endPoints.resize(2);
			allPoints[i].endPoints[0] = { 2 };
			allPoints[i].endPoints[1] = { 5 };
		} else if (i == 2) {
			allPoints[i].endPoints.resize(2);
			allPoints[i].endPoints[0] = { 3 };
			allPoints[i].endPoints[1] = { 6 };
		} else if (i == 3) {
			allPoints[i].endPoints.resize(1);
			allPoints[i].endPoints[0] = { 7 };
		} else if (i == 4) {
			allPoints[i].endPoints.resize(2);
			allPoints[i].endPoints[0] = { 5 };
			allPoints[i].endPoints[1] = { 7 };
		} else if (i == 5) {
			allPoints[i].endPoints.resize(1);
			allPoints[i].endPoints[0] = { 6 };
		} else if (i == 6) {
			allPoints[i].endPoints.resize(1);
			allPoints[i].endPoints[0] = { 7 };
		} else if (i == 7) {
			allPoints[i].endPoints.resize(0);
		}
		

	}
	if (redraw) {
		//Clear previous drawing
		RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
	}
	
	//Draw!
	MoveToEx(hDC, allPoints[0].xValue, allPoints[0].yValue, NULL);
	for (i = 0; i < numPoints; i++) {
		for (j = 0; j < allPoints[i].endPoints.size(); j++) {
			MoveToEx(hDC, allPoints[i].xValue, allPoints[i].yValue, NULL);
			int endPoint = allPoints[i].endPoints[j];
			LineTo(hDC, allPoints[endPoint].xValue, allPoints[endPoint].yValue);

		}
		//OutputDebugStringA("LOOP");
		if (i == 7) {
			//OutputDebugStringA("\n \n");
		}
	}

}

//The following was (heavily) based on the C++ tutorials from:
//https://www.cprogramming.com/tutorial/opengl_first_windows_app.html
//and 
//http://www.winprog.org/tutorial/simple_window.html

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	//Device Context
	HDC hDC;


	
	//OutputDebugStringA("yDelta increased \n");

	switch (message)
	{
		/*  Window is being created*/
	case WM_CREATE:
		return 0;
		break;
		/*  Window is closing*/
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

		//The important bit - runs this on redraw
	case WM_PAINT:
		//Start painting
		hDC = BeginPaint(hwnd, &paintStruct);

		//Custom function, from above
		TestPaint(hDC, hwnd, false);
		//Stop painting

		EndPaint(hwnd, &paintStruct);
		return 0;
		break;

	case WM_TIMER:
		OutputDebugStringA("Regular Timer called \n");
		switch (wParam)
		{
		case 500:
			// process the 10-second timer 
			yDeltaAngle += 0.01;
			OutputDebugStringA("Actual timer called \n");

			hDC = BeginPaint(hwnd, &paintStruct);

			//Custom function, from above
			TestPaint(hDC, hwnd, true);
			//Stop painting

			EndPaint(hwnd, &paintStruct);
			return 0;
		}
		// process the 10-second timer 
		
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}



// 'Main function'
//This kinds of makes sense, but I'm still not getting most of the intracies
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{

	WNDCLASSEX  windowClass;        //window class
	HWND        hwnd;               //window handle
	MSG         msg;                //message
	bool        done;               //flag saying when app is complete
	/*  Fill out the window class structure*/
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"MyClass";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	/*  Register window class*/
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}
	/*  Class registered, so now create window*/
	hwnd = CreateWindowEx(NULL,     //extended style
		L"MyClass",          //class name
		L"SimpleRender",       //app name
		WS_OVERLAPPEDWINDOW |       //window style
		WS_VISIBLE |
		WS_SYSMENU,
		250, 250,            //x/y coords of the new window itself on the desktop
		400, 400,            //width,height of the new window (default - can be resized!)
		NULL,               //handle to parent
		NULL,               //handle to menu
		hInstance,          //application instance
		NULL);              //no extra parameter's

	//  Check if window creation failed
	if (!hwnd)
		return 0;
	done = false; //initialize loop condition variable

	SetTimer(hwnd,             // handle to main window 
		500,            // timer identifier 
		1,                 // 1-ms interval(?) 
		(TIMERPROC)NULL);     // no timer callback 


	//  main message loop
	while (!done)
	{
		PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
		if (msg.message == WM_QUIT) //check for a quit message
		{
			done = true; //if found, quit app
		}
		else
		{
			//  Translate and dispatch to event queue
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}
	OutputDebugStringA("Exit msg loop \n");
	return msg.wParam;
	
}

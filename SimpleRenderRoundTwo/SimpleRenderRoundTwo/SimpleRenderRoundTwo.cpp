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
float yAngularVelocity = 0.01;
float centerPoint[3] = { 180, 215, 140 };
int xVel = 1;
int yVel = 1;
int zVel = 1;

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
		////Calculate effects of rotation, relative to centerPoint
		//Get hypotenuse lengths
		float XZLength = sqrt(pow(xValues[i], 2) + pow(zValues[i], 2));

		//Find initial angle
		float yInitAngle = acos(xValues[i] / XZLength);
		if (zValues[i] < 0) {
			yInitAngle += 3.14159265358979323846;
		}


		//Find x, z values
		float rotatedX = XZLength * cos(yInitAngle + yDeltaAngle);
		float rotatedZ = XZLength * sin(yInitAngle + yDeltaAngle);

		////Translate based on centerpoint
		float translatedX = rotatedX + centerPoint[0];
		float translatedY = yValues[i] + centerPoint[1];
		float translatedZ = rotatedZ + centerPoint[2];


		////Calculate draw locations based on perspective
		allPoints[i].xValue = cameraX + ((translatedX - cameraX) * cameraZ) / (cameraZ + translatedZ);
		allPoints[i].yValue = cameraY + ((translatedY - cameraY) * cameraZ) / (cameraZ + translatedZ);


		//Assign lineNodes - do this once
		if (i == 0) {
			allPoints[i].endPoints.resize(4);
			allPoints[i].endPoints[0] = { 1 };
			allPoints[i].endPoints[1] = { 2 };
			allPoints[i].endPoints[2] = { 3 };
			allPoints[i].endPoints[3] = { 4 };
		} else if (i == 1) {
			allPoints[i].endPoints.resize(3);
			allPoints[i].endPoints[0] = { 2 };
			allPoints[i].endPoints[1] = { 3 };
			allPoints[i].endPoints[2] = { 5 };
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

		//Move the box!
		centerPoint[0] += xVel;
		centerPoint[1] += yVel;
		centerPoint[2] += zVel;
		if (centerPoint[0] > 325 || centerPoint[0] < 80) {
			xVel = -xVel;
		}
		if (centerPoint[1] > 320 || centerPoint[1] < 80) {
			yVel = -yVel;
		}
		if (centerPoint[2] > 450 || centerPoint[2] < 100) {
			zVel = -zVel;
		}
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



//Used to make the button click do something
WNDPROC LinkButtonProc;

//Callback for button
LRESULT CALLBACK ButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//OutputDebugStringA( message);
	switch (message)
	{
	case WM_LBUTTONDOWN:
		yAngularVelocity = -yAngularVelocity;
		return 0;
		break;
	}


	return (CallWindowProc(LinkButtonProc, hwnd, message, wParam, lParam));
}

//The following was (heavily) based on the C++ tutorials from:
//https://www.cprogramming.com/tutorial/opengl_first_windows_app.html
//and 
//http://www.winprog.org/tutorial/simple_window.html

//Callback for drawing
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	//Device Context
	HDC hDC;


	
	//OutputDebugStringA("yDelta increased \n");

	switch (message)
	{
		/*  Window is being created*/
	case WM_CREATE: {
		HWND hwndButton = CreateWindow(
			L"BUTTON",
			L"Change Rotation",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			10,         // y position 
			120,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			NULL,		//(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE)
			NULL);      // Pointer not needed.
		LinkButtonProc = (WNDPROC)SetWindowLong(hwndButton, GWL_WNDPROC, (LONG)ButtonProc);
		OutputDebugStringA("New button \n");

		return 0;
		break;
	}
		
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
		switch (wParam)
		{
		case 500:
			//The timer is called
			yDeltaAngle += yAngularVelocity;

			hDC = BeginPaint(hwnd, &paintStruct);

			//Custom function, from above
			TestPaint(hDC, hwnd, true);
			//Stop painting

			EndPaint(hwnd, &paintStruct);
			return 0;
		}
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
	//  Register window class
	RegisterClassEx(&windowClass);

	//Create the window 
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

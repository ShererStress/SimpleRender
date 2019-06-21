using namespace std;
/*  Trim fat from windows*/
//#define WIN32_LEAN_AND_MEAN
#pragma comment(linker, "/subsystem:windows")
/*  Pre-processor directives*/

#include <windows.h>
#include <iostream>
#include <typeinfo>
#include <vector>



void TestPaint(HDC hDC)
{

	OutputDebugStringA("CCCC \n");


	const int numPoints = 8;
	int cameraX = 200;
	int cameraY = 200;
	int cameraZ = 200;


	struct drawPoint {
		int xValue;
		int yValue;
		int zValue;
		vector<int> endPoints;
	} allPoints[numPoints];


	int xValues[numPoints] = { 100, 300, 300, 100, 100, 300, 300, 100 };
	int yValues[numPoints] = { 100, 100, 300, 300, 100, 100, 300, 300 };
	int zValues[numPoints] = { 400, 400, 400, 400, 200, 200, 200, 200 };
	//int linesToDraw = new int[3];
	//string linesToDraw[numPoints] = { "1,3","2","","" };

	//Assign values to the drawPoint structs

	int i;
	int j;
	for (i = 0; i < numPoints; i++) {
		allPoints[i].xValue = cameraX + ((xValues[i] - cameraX) * cameraZ) / (cameraZ + zValues[i]);
		allPoints[i].yValue = cameraY + ((yValues[i] - cameraY) * cameraZ) / (cameraZ + zValues[i]);
		//Assign lineNodes
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

	

	//Draw!
	MoveToEx(hDC, allPoints[0].xValue, allPoints[0].yValue, NULL);
	for (i = 0; i < numPoints; i++) {
		for (j = 0; j < allPoints[i].endPoints.size(); j++) {
			MoveToEx(hDC, allPoints[i].xValue, allPoints[i].yValue, NULL);
			int endPoint = allPoints[i].endPoints[j];
			LineTo(hDC, allPoints[endPoint].xValue, allPoints[endPoint].yValue);

		}
		OutputDebugStringA("LOOP");
	}
	//LineTo(hDC, allPoints[0].xValue, allPoints[0].yValue);

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	/*  Device Context*/
	HDC hDC;


	/*  Switch message, condition that is met will execute*/
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
		/*  Window needs update*/
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);

		TestPaint(hDC);
		EndPaint(hwnd, &paintStruct);
		return 0;
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}






/*  Main function*/
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
	/*  Class registerd, so now create window*/
	hwnd = CreateWindowEx(NULL,     //extended style
		L"MyClass",          //class name
		L"A Real Win App",       //app name
		WS_OVERLAPPEDWINDOW |       //window style
		WS_VISIBLE |
		WS_SYSMENU,
		100, 100,            //x/y coords
		400, 400,            //width,height
		NULL,               //handle to parent
		NULL,               //handle to menu
		hInstance,          //application instance
		NULL);              //no extra parameter's
	/*  Check if window creation failed*/
	if (!hwnd)
		return 0;
	done = false; //initialize loop condition variable
	/*  main message loop*/
	while (!done)
	{
		PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
		if (msg.message == WM_QUIT) //check for a quit message
		{
			done = true; //if found, quit app
		}
		else
		{
			/*  Translate and dispatch to event queue*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

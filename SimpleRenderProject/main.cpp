using namespace std;
/*  Trim fat from windows*/
//#define WIN32_LEAN_AND_MEAN
#pragma comment(linker, "/subsystem:windows")
/*  Pre-processor directives*/

#include <windows.h>
#include <iostream>
/*  Windows Procedure Event Handler*/



void TestPaint(HDC hDC)
{

    const int numPoints = 4;
    int cameraX = 200;
    int cameraY = 200;
    int cameraZ = 200;

    struct lineNode {
        int endPoint;
        lineNode *next;
    };

    struct drawPoint {
        int xValue;
        int yValue;
        int zValue;
        lineNode firstNode;
    } allPoints[numPoints];


    //Abstract this to external file... eventually
    int xValues[numPoints] = {100, 300, 300, 100};
    int yValues[numPoints] = {100, 100, 300, 300};
    int zValues[numPoints] = {400, 400, 400, 400};
    string linesToDraw[numPoints] = {"1,3","2","",""};

    //Assign values to the drawPoint structs
    int i;
    for ( i = 0; i < numPoints; i++) {
        allPoints[i].xValue = cameraX+((xValues[i]-cameraX)*cameraZ)/(cameraZ+zValues[i]);
        allPoints[i].yValue = cameraY+((yValues[i]-cameraY)*cameraZ)/(cameraZ+zValues[i]);
        cout << allPoints[i].xValue;
        cout << endl;
        cout << allPoints[i].yValue;
        cout << endl;
        cout << endl;
        //Assign lineNodes
        int j;
        for(j = 0; j < linesToDraw[i].length(); j++) {
            int thisNum = -1;
            char thisChar = linesToDraw[i].at(j);
            string delimiterChar = ",";
            if( delimiterChar.at(0) != thisChar) {
                thisNum = linesToDraw[i].at(j) - "0";
            }

            cout << "THIS CHAR:";
            cout << thisChar;
            cout << endl;
        }
    }

    //Draw!
    MoveToEx(hDC, allPoints[0].xValue, allPoints[0].yValue, NULL);
    for ( i = 1; i < numPoints; i++) {
        LineTo(hDC, allPoints[i].xValue, allPoints[i].yValue);

    }
    LineTo(hDC, allPoints[0].xValue, allPoints[0].yValue);

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paintStruct;
    /*  Device Context*/
    HDC hDC;
    /*  Text for display*/
    char string[] = "Hello, World!";
    /*  Switch message, condition that is met will execute*/
    switch(message)
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
            hDC = BeginPaint(hwnd,&paintStruct);
            /*  Set txt color to blue*/
            SetTextColor(hDC, COLORREF(0x00FF0000));
            /*  Display text in middle of window*/
            TextOut(hDC,150,150,string,sizeof(string)-1);
            TestPaint(hDC);
            EndPaint(hwnd, &paintStruct);
            return 0;
            break;
        default:
            break;
    }
    return (DefWindowProc(hwnd,message,wParam,lParam));
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
    windowClass.lpszClassName = "MyClass";
    windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    /*  Register window class*/
    if (!RegisterClassEx(&windowClass))
    {
        return 0;
    }
    /*  Class registerd, so now create window*/
    hwnd = CreateWindowEx(NULL,     //extended style
        "MyClass",          //class name
        "A Real Win App",       //app name
        WS_OVERLAPPEDWINDOW |       //window style
        WS_VISIBLE |
        WS_SYSMENU,
        100,100,            //x/y coords
        400,400,            //width,height
        NULL,               //handle to parent
        NULL,               //handle to menu
        hInstance,          //application instance
        NULL);              //no extra parameter's
    /*  Check if window creation failed*/
    if (!hwnd)
        return 0;
    done = false; //initialize loop condition variable
    /*  main message loop*/
    while(!done)
    {
        PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
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

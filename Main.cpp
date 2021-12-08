//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>
#include "Windows.h"
#include "MMSystem.h"
#include <string>
#include <fstream>

#define MAX_LOADSTRING 200

// Global Variables:
MyImage			inImage, outImage;				// image objects
MyImage			originIn, originOut;
char			SoundPath[_MAX_PATH];			// sound wav file
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void 				ColorPixel(char* imgBuf, int w, int h, int x, int y);
void				DrawLine(char* imgBuf, int w, int h, int x1, int y1, int x2, int y2);

int trackbar1Pos = 1, trackbar2Pos = 1; //video timeline
char* newFramePath1 = new char[_MAX_PATH];
char* newFramePath2 = new char[_MAX_PATH];

int startX = 0, startY = 0, endX = 0, endY = 0;
int boxB = 0, boxG = 0, boxR = 0;

std::string hyperlink = "";

std::ofstream outputFile;

//different display modes
enum class LCLICKSTATUS { LDOWN = 1, LUP = 2 };
LCLICKSTATUS lclickStatus = LCLICKSTATUS::LUP;
enum class RCLICKSTATUS { RDOWN = 1, RUP = 2 };
RCLICKSTATUS rclickStatus = RCLICKSTATUS::RUP;

enum class LINKSTATUS { NORMAL = 1, LINKING = 2 };
LINKSTATUS linkStatus = LINKSTATUS::NORMAL;

// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	MSG msg;
	HACCEL hAccelTable;

	// Read in the image and its copy
	int w, h;
	char ImagePath[_MAX_PATH];

	sscanf(lpCmdLine, "%s %d %d %s", &ImagePath, &w, &h, &SoundPath);
	inImage.setWidth(w);
	inImage.setHeight(h);
	outImage.setWidth(w);
	outImage.setHeight(h);
	strcpy(newFramePath1, "");
	strcpy(newFramePath2, "");
	/*if (strstr(ImagePath, ".rgb") == NULL) {
		AfxMessageBox("Image has to be a '.rgb' file\nUsage - Image.exe image.rgb w h sound.wav");
		//return FALSE;
	}
	else {
		inImage.setImagePath(ImagePath);
		if (!inImage.ReadImage()) {
			AfxMessageBox("Could not read image\nUsage - Image.exe image.rgb w h sound.wav");
		}
		else {
			originIn = inImage;
			strcpy(newFramePath1, inImage.getImagePath());
		}
		outImage.setImagePath("AIFilmTwo\\AIFilmTwo0001.rgb");
		if (!outImage.ReadImage()) {
			AfxMessageBox("Could not read right image");
		}
		else {
			originOut = outImage;
			strcpy(newFramePath2, outImage.getImagePath());
		}

	}*/

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

// Colors a pixel at the given (x, y) position black.
void ColorPixel(char* imgBuf, int w, int h, int x, int y) {
	imgBuf[(4 * y * w) + 4 * x] = 0;
	imgBuf[(4 * y * w) + 4 * x + 1] = 0;
	imgBuf[(4 * y * w) + 4 * x + 2] = 0;
}

// Draws a line using Bresenham's algorithm.
void DrawLine(char* imgBuf, int w, int h, int x1, int y1, int x2, int y2) {
	const bool steep = (abs(y2 - y1) > abs(x2 - x1));
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs((float)(y2 - y1));

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x < maxX; x++) {
		if (steep) {
			ColorPixel(imgBuf, w, h, y, x);
		}
		else {
			ColorPixel(imgBuf, w, h, x, y);
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// CreateTrackbar - creates and initializes a trackbar. 
// 
// Global variable
//     g_hinst - instance handle
//
HWND WINAPI CreateTrackbar(
	HWND hwndDlg,  // handle of dialog box (parent window) 
	UINT iMin,     // minimum value in trackbar range 
	UINT iMax,     // maximum value in trackbar range 
	UINT iSelMin,  // minimum value in trackbar selection 
	UINT iSelMax, // maximum value in trackbar selection 
	UINT positionX,
	UINT positionY,
	UINT trackbarID) {

	InitCommonControls(); // loads common control's DLL 

	HWND hwndTrack = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		"Trackbar Control",              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_NOTICKS |
		TBS_ENABLESELRANGE,              // style 
		positionX, positionY,                          // position 
		300, 30,                         // size 
		hwndDlg,                         // parent window 
		(HMENU)trackbarID,                     // control identifier 
		NULL,                         // instance 
		NULL                             // no WM_CREATE parameter 
	);

	SendMessage(hwndTrack, TBM_SETRANGE,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
				0, (LPARAM)4);                  // new page size 

	SendMessage(hwndTrack, TBM_SETSEL,
				(WPARAM)FALSE,                  // redraw flag 
				(LPARAM)MAKELONG(iSelMin, iSelMax));

	SendMessage(hwndTrack, TBM_SETPOS,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)iSelMin);

	SetFocus(hwndTrack);

	return hwndTrack;
}

void openFile(HWND hWnd, bool left) {
	OPENFILENAME ofn;

	char* fileName = new char[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "Video files\0*0001.rgb";
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);

	if (left) {
		inImage.setImagePath(ofn.lpstrFile);
		if (!inImage.ReadImage()) {
			AfxMessageBox("Could not read image\nUsage - Image.exe image.rgb w h sound.wav");
		}
		else {
			originIn = inImage;
			strcpy(newFramePath1, inImage.getImagePath());
		}
	}
	else {
		outImage.setImagePath(ofn.lpstrFile);
		if (!outImage.ReadImage()) {
			AfxMessageBox("Could not read image\nUsage - Image.exe image.rgb w h sound.wav");
		}
		else {
			originOut = outImage;
			strcpy(newFramePath2, outImage.getImagePath());
		}
	}
	
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// TO DO: part useful to render video frames, may place your own code here in this function
	// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	int length1, length2;
	int mousePositionX, mousePositionY;
	std::string pos1, pos2;

	switch (message) {
	case WM_CREATE:
		CreateWindow(TEXT("button"), TEXT("select left video"),
					 WS_VISIBLE | WS_CHILD,
					 10, 60, 150, 25,
					 hWnd, (HMENU)ID_BUTTONSELECTL, NULL, NULL);
		CreateWindow(TEXT("button"), TEXT("select right video"),
					 WS_VISIBLE | WS_CHILD,
					 180, 60, 150, 25,
					 hWnd, (HMENU)ID_BUTTONSELECTR, NULL, NULL);
		CreateWindow(TEXT("button"), TEXT("create new link"),
					 WS_VISIBLE | WS_CHILD,
					 350, 60, 150, 25,
					 hWnd, (HMENU)ID_BUTTONCREATE, NULL, NULL);
		CreateWindow(TEXT("button"), TEXT("connect video"),
					 WS_VISIBLE | WS_CHILD,
					 520, 60, 150, 25,
					 hWnd, (HMENU)ID_BUTTONCONNECT, NULL, NULL);
		CreateWindow(TEXT("button"), TEXT("save file"),
					 WS_VISIBLE | WS_CHILD,
					 690, 60, 150, 25,
					 hWnd, (HMENU)ID_BUTTONSAVEFILE, NULL, NULL);
		CreateTrackbar(hWnd, 1, 9000, 1, 9000, 10, 400, ID_TRACKBAR1);
		CreateTrackbar(hWnd, 1, 9000, 1, 9000, outImage.getWidth() + 60, 400, ID_TRACKBAR2);
		break;
	case WM_HSCROLL:
		trackbar1Pos = SendMessage(GetDlgItem(hWnd, ID_TRACKBAR1), TBM_GETPOS, 0, 0);
		trackbar2Pos = SendMessage(GetDlgItem(hWnd, ID_TRACKBAR2), TBM_GETPOS, 0, 0);

		length1 = strlen(inImage.getImagePath());
		length2 = strlen(outImage.getImagePath());
		strcpy(newFramePath1, inImage.getImagePath());
		strcpy(newFramePath2, outImage.getImagePath());
		pos1 = std::to_string(trackbar1Pos);
		pos2 = std::to_string(trackbar2Pos);
		while (pos1.size() < 4)
			pos1 = "0" + pos1;
		while (pos2.size() < 4)
			pos2 = "0" + pos2;
		for (int i = 0; i < 4; i++) {
			newFramePath1[length1 - 8 + i] = pos1[i];
			newFramePath2[length2 - 8 + i] = pos2[i];
		}

		inImage.setImagePath(newFramePath1);
		if (!inImage.ReadImage())
			AfxMessageBox("Could not read left image");
		else
			originIn = inImage;

		outImage.setImagePath(newFramePath2);
		if (!outImage.ReadImage())
			AfxMessageBox("Could not read right image");
		else
			originOut = outImage;

		RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case ID_BUTTONSELECTL:
			openFile(hWnd, true);
			//RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
			InvalidateRect(hWnd, &rt, false);
			break;
		case ID_BUTTONSELECTR:
			openFile(hWnd, false);
			//RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
			InvalidateRect(hWnd, &rt, false);
			break;
		case ID_BUTTONCREATE:
			linkStatus = LINKSTATUS::LINKING;
			srand(time(NULL));
			boxB = rand() % 205 + 50, boxG = rand() % 205 + 50, boxR = rand() % 205 + 50; //randomize bounding box color
			break;
		case ID_BUTTONCONNECT:
			hyperlink += "\n" + std::to_string(trackbar1Pos) + " " + std::to_string(trackbar2Pos);
			hyperlink += " " + std::to_string(startX) + " " + std::to_string(startY);
			hyperlink += " " + std::to_string(endX) + " " + std::to_string(endY);
			//std::cout << hyperlink << std::endl;
			//hyperlink = ""; //reset
			originIn = inImage;
			linkStatus = LINKSTATUS::NORMAL;
			break;
		case ID_BUTTONSAVEFILE:
			std::cout << "yes";
			outputFile.open("hyperlink.txt");
			outputFile << inImage.getImagePath();
			outputFile << hyperlink;
			outputFile.close();
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case ID_MODIFY_IMAGE:
			PlaySound(TEXT(SoundPath), NULL, SND_ASYNC);			// New addition to the code to play a wav file
			outImage.Modify();
			InvalidateRect(hWnd, &rt, false);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		mousePositionX = GET_X_LPARAM(lParam);
		mousePositionY = GET_Y_LPARAM(lParam);

		if (linkStatus == LINKSTATUS::LINKING) {
			if (mousePositionX >= 0 && mousePositionX < 0 + inImage.getWidth() &&
				mousePositionY >= 100 && mousePositionY < 100 + inImage.getHeight()) {		// click on left video
				startX = mousePositionX, startY = mousePositionY - 100;
				lclickStatus = LCLICKSTATUS::LDOWN;
			}
			else if (mousePositionX >= 50 + inImage.getWidth() && mousePositionX < 50 + inImage.getWidth() + outImage.getWidth() &&
					 mousePositionY >= 100 && mousePositionY < 100 + outImage.getHeight()) { // click on right video
				startX = mousePositionX - 50 - inImage.getWidth(), startY = mousePositionY - 100;
				rclickStatus = RCLICKSTATUS::RDOWN;
			}

			RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case WM_MOUSEMOVE:
		// modifying bounding box
		if (lclickStatus == LCLICKSTATUS::LDOWN) {
			mousePositionX = LOWORD(lParam);
			if (mousePositionX > 0 + inImage.getWidth())
				mousePositionX = 0 + inImage.getWidth() - 1;
			else if (mousePositionX < 0)
				mousePositionX = 0;
			mousePositionY = HIWORD(lParam);
			if (mousePositionY > 100 + inImage.getHeight())
				mousePositionY = 100 + inImage.getHeight() - 1;
			else if (mousePositionY < 100)
				mousePositionY = 100;
			inImage = originIn;
			inImage.TargetArea(startX, startY, mousePositionX, mousePositionY - 100, boxB, boxG, boxR);
			RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		else if (rclickStatus == RCLICKSTATUS::RDOWN) {
			mousePositionX = LOWORD(lParam);
			if (mousePositionX > 50 + inImage.getWidth() + outImage.getWidth())
				mousePositionX = 50 + inImage.getWidth() + outImage.getWidth() - 1;
			else if (mousePositionX < 50 + inImage.getWidth())
				mousePositionX = 50 + inImage.getWidth();
			mousePositionY = HIWORD(lParam);
			if (mousePositionY > 100 + outImage.getHeight())
				mousePositionY = 100 + outImage.getHeight() - 1;
			else if (mousePositionY < 100)
				mousePositionY = 100;
			outImage = originOut;
			outImage.TargetArea(startX, startY, mousePositionX - 50 - inImage.getWidth(), mousePositionY - 100, boxB, boxG, boxR);
			RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case WM_LBUTTONUP:
		// get final bounding box coordinates
		if (lclickStatus == LCLICKSTATUS::LDOWN) {
			mousePositionX = LOWORD(lParam);
			if (mousePositionX > 0 + inImage.getWidth())
				mousePositionX = 0 + inImage.getWidth() - 1;
			else if (mousePositionX < 0)
				mousePositionX = 0;
			mousePositionY = HIWORD(lParam);
			if (mousePositionY > 100 + inImage.getHeight())
				mousePositionY = 100 + inImage.getHeight() - 1;
			else if (mousePositionY < 100)
				mousePositionY = 100;

			endX = mousePositionX, endY = mousePositionY;
		}
		else if (rclickStatus == RCLICKSTATUS::RDOWN) {
			mousePositionX = LOWORD(lParam);
			if (mousePositionX > 50 + inImage.getWidth() + outImage.getWidth())
				mousePositionX = 50 + inImage.getWidth() + outImage.getWidth() - 1;
			else if (mousePositionX < 50 + inImage.getWidth())
				mousePositionX = 50 + inImage.getWidth();
			mousePositionY = HIWORD(lParam);
			if (mousePositionY > 100 + outImage.getHeight())
				mousePositionY = 100 + outImage.getHeight() - 1;
			else if (mousePositionY < 100)
				mousePositionY = 100;

			endX = mousePositionX, endY = mousePositionY;
		}

		lclickStatus = LCLICKSTATUS::LUP;
		rclickStatus = RCLICKSTATUS::RUP;
		RedrawWindow(hWnd, &rt, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TO DO: Add any drawing code here...
		char text[1000];
		//strcpy(text, "Original image (Left)  Image after modification (Right)\n");
		//strcpy(text, "\nUpdate program with your code to modify input image");
		strcpy(text, newFramePath1);
		DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
		rt.top = 30;
		strcpy(text, newFramePath2);
		DrawText(hdc, text, strlen(text), &rt, DT_LEFT);

		BITMAPINFO bmi;
		CBitmap bitmap;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = inImage.getWidth();
		bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = inImage.getWidth() * inImage.getHeight();

		SetDIBitsToDevice(hdc,
						  0, 100, inImage.getWidth(), inImage.getHeight(),
						  0, 0, 0, inImage.getHeight(),
						  inImage.getImageData(), &bmi, DIB_RGB_COLORS);

		SetDIBitsToDevice(hdc,
						  outImage.getWidth() + 50, 100, outImage.getWidth(), outImage.getHeight(),
						  0, 0, 0, outImage.getHeight(),
						  outImage.getImageData(), &bmi, DIB_RGB_COLORS);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
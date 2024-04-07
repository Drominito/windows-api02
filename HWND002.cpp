// beim Callback Methode wird z.B. die Maus nur geändert auch nur wenn es auf dem Window ist, aber wo wird es genau bestimmt?
// the flow should be not only for one axis, it should be x and y!!


#ifdef PCPU_DEBUG

#define DEBUG 1

#else

#define DEBUG 0

#endif


#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

#include "input.h"

using namespace std::literals::chrono_literals;
#define tSleep(t) std::this_thread::sleep_for(t * (1ms))
#define mcs(str) #str // mcs -> MarcoString -> so the variable name as string


LRESULT CALLBACK winProc(HWND hWin, UINT msg, WPARAM wp, LPARAM lp);
int MovingWindow(HWND& hWin);

struct WindowStuff
{
	Input input;

	bool running = true;
	bool animation = false;
};
WindowStuff windowStuff;

enum IterationFlow
{
	Negative,
	Positive
};

struct NTString // Null-Terminated String
{
	UINT* variableString;
	bool nt;
};


int main()
{
#if DEBUG

	const char* data = "Hello World!\n";

	[&]()
	{
		printf("The %s is %s", mcs(data), data);
	}();
#endif

	int a = sizeof(char[5]);
	int b = sizeof(wchar_t[5]);

	WNDCLASS wc = { sizeof(WNDCLASS) }; // its a style for an window, but not how it looks, rather how it behaves

	wc.hCursor	     = LoadCursor(0, IDC_ARROW);
	wc.hInstance     = GetModuleHandle(0);
	wc.lpszClassName = "SomeClassName";
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = winProc;

	if (!RegisterClass(&wc))
		return 0;

	
	HWND hWin = CreateWindowA(
		wc.lpszClassName,
		"SomeWindow",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		200,
		200,
		NULL, NULL,
		GetModuleHandle(0), 0);

	
	

	while (windowStuff.running)
	{
		//std::cout << "Window is : Open" << '\n';
		MSG msg = {};
		bool EventQue;
		static int ColorI = 0;

		(PeekMessage(&msg, hWin, 0, 0, PM_REMOVE) > 0) ? EventQue = true : EventQue = false;

		while (EventQue)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			(PeekMessage(&msg, hWin, 0, 0, PM_REMOVE) > 0) ? EventQue = true : EventQue = false;
		}

		if (!EventQue)
		{
			if (windowStuff.animation)
			{
				int result = MovingWindow(hWin);
				if (result == 1) // one as resturned error -> (1)
					MessageBox(hWin, "Nope, you have some error", "R.I.P", MB_OK);
			}
		}


	}

	std::cout << "Window is : Closed" << '\n';
	return 0;

}


LRESULT CALLBACK winProc(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	/*RECT rect;
	POINT cursor;*/
	LRESULT result = 0;


	HCURSOR hCursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(hCursor);
	
		
	if (GetAsyncKeyState(VK_RETURN))
	{
		windowStuff.animation = true;
		
	}
	if (GetAsyncKeyState('A') & 1)
	{
		DWORD style = GetClassLongA(hWin, GCL_STYLE);
		style = CS_DBLCLKS;
		SetClassLongA(hWin, GCL_STYLE, style);
		MessageBox(hWin, "smth", "", MB_OK);
	}

	

	switch (msg)
	{
		case WM_CREATE:
		{
			// stuff for later
			break;
		}

		case WM_CLOSE:
		{
			const char* txt = "Are you sure you want to close the Program?";
			const char* titleText = "Confirm to close.";
			int MB_ID;

			// Zeige die MessageBox auf dem Hauptfenster an
			MB_ID = MessageBox(hWin, txt, titleText, MB_YESNO);


			if (MB_ID == IDYES)
			{
				windowStuff.running = false;
				HWND hCmd = GetConsoleWindow();
				if (hCmd != NULL)
				{
					ShowWindow(hWin, SW_HIDE);
					ShowWindow(hCmd, SW_SHOWMAXIMIZED);

					for (int i = 3; i > 0; --i)
					{
						printf("The Programm will close in %i\n", i);
						tSleep(1000); //1s
					}
					PostMessage(hCmd, WM_CLOSE, NULL, NULL);
				}
			}

			break;
		}

		case WM_MBUTTONDOWN: { windowStuff.animation = true;  break;};
		case WM_RBUTTONDOWN: { windowStuff.animation = false; break;};
		

		case WM_SIZE:
		{
			bool previousState = false;
			bool currentState = ( GetAsyncKeyState(VK_LBUTTON) == 0);
			bool mouseReleased = false;
			(currentState != previousState) ? mouseReleased = true : mouseReleased = false;

			if (mouseReleased)
			{
				//Refresh
				tSleep(50);
				ShowWindow(hWin, SW_HIDE);
				tSleep(50);
				ShowWindow(hWin, SW_SHOW);

			}
		}

		default:
		{
			result = DefWindowProc(hWin, msg, wp, lp);
			break;
		}
	
	}

	
	

	return result;
}


int MovingWindow(HWND& hWin)
{
	MONITORINFO monitorInfo; monitorInfo.cbSize = sizeof(MONITORINFO);

	HMONITOR hMonitor = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

	if (!GetMonitorInfo(hMonitor, &monitorInfo))
	{
		LPSTR msgBuff = nullptr;
		DWORD dlgErr = GetLastError();
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dlgErr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPSTR)&msgBuff, 0, NULL);

		std::string message;
		if (size != 0)
		{
			message.assign(msgBuff, size);
			LocalFree(msgBuff);

			if (dlgErr != 0)
			{
				MessageBox(hWin, message.c_str(), NULL, MB_OK);
			}
			return 1;
		}
		else
		{
			std::string tmpMsg = "Error: " + std::to_string(GetLastError());
			MessageBoxA(hWin, tmpMsg.c_str(), "Error-Handling-Error", MB_OK);
			return 1;
		}
	}

	RECT displayRect = monitorInfo.rcMonitor;


	RECT windowRect = {};
	GetWindowRect(hWin, &windowRect);
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;
	static float offsetX = 0;
	static int offsetY = 0;

	static BOOL flowX = IterationFlow::Positive;
	static BOOL flowY = IterationFlow::Positive;

	static int moduloi;

	switch (flowX)
	{
		case IterationFlow::Positive:
		{
			if (windowWidth + offsetX < displayRect.right - 50)
			{
				++offsetX;
			}
			else if (windowWidth + offsetX >= displayRect.right - 50)
			{
				flowX = IterationFlow::Negative;
			}

			break;
		}
		case IterationFlow::Negative:
		{
			if (windowWidth + offsetX > 1 && windowWidth + offsetX <= displayRect.right - 50)
			{
				--offsetX;
			}
			else if (windowWidth + offsetX > 0 && windowWidth + offsetX < 10)
			{
				flowX = IterationFlow::Positive;
			}

			break;
		}

	}

	switch (flowY)
	{
		case IterationFlow::Positive:
		{
			if (windowHeight + offsetY < displayRect.bottom - 50)
			{
				++offsetY;
			}
			else if (windowHeight + offsetY >= displayRect.bottom - 50)
			{
				flowY = IterationFlow::Negative;
			}
			break;

		}
		case IterationFlow::Negative:
		{
			if (windowHeight + offsetY > 50)
			{
				--offsetY;
			}
			else // windowHeight + offsetY < 50
			{
				flowY = IterationFlow::Positive;
			}

			break;
		}
	}


	SetWindowPos(hWin, HWND_TOPMOST, offsetX, offsetY, windowWidth, windowHeight, NULL);

	//tSleep(25);

	return 0;
}
#include "main_wnd.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// create windows
	MainWnd wnd("127.0.0.1", 8888, FALSE, FALSE);
	if (!wnd.Create()) {
		return -1;
	}

	// main loop
	MSG msg;
	BOOL gm;
	while ((gm = GetMessage(&msg, NULL, 0, 0)) != 0 && gm != -1) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}
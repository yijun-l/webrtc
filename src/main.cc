#include "main_wnd.h"
#include "peer_connection_client.h"
#include "conductor.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// create windows
	MainWnd wnd("127.0.0.1", 8888, FALSE, FALSE);
	if (!wnd.Create())
	{
		return -1;
	}

	PeerConnectionClient client;
	// auto conductor = rtc::make_ref_counted<Conductor>(&client, &wnd);
	Conductor conductor(&client, &wnd);

	// main loop
	MSG msg;
	BOOL gm;
	while ((gm = GetMessage(&msg, NULL, 0, 0)) != 0 && gm != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
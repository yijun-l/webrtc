#include "main_wnd.h"

namespace
{

	HFONT GetDefaultFont()
	{
		static HFONT font = reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
		return font;
	}

	void CalculateWindowSizeForText(HWND wnd,
									const wchar_t *text,
									size_t *width,
									size_t *height)
	{
		HDC dc = ::GetDC(wnd);
		RECT text_rc = {0};
		::DrawTextW(dc, text, -1, &text_rc, DT_CALCRECT | DT_SINGLELINE);
		::ReleaseDC(wnd, dc);
		RECT client, window;
		::GetClientRect(wnd, &client);
		::GetWindowRect(wnd, &window);

		*width = text_rc.right - text_rc.left;
		*width += (window.right - window.left) - (client.right - client.left);
		*height = text_rc.bottom - text_rc.top;
		*height += (window.bottom - window.top) - (client.bottom - client.top);
	}
}

ATOM MainWnd::wnd_class_ = 0;
const wchar_t MainWnd::kClassName[] = L"WebRTC_MainWnd";

MainWnd::MainWnd(const char *server, int port, bool auto_connect, bool auto_call)
	: ui_(CONNECT_TO_SERVER),
	  wnd_(NULL),
	  edit1_(NULL),
	  edit2_(NULL),
	  label1_(NULL),
	  label2_(NULL),
	  button_(NULL),
	  listbox_(NULL),
	  callback_(NULL),
	  server_(server),
	  auto_connect_(auto_connect),
	  auto_call_(auto_call)
{
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%i", port);
	port_ = buffer;
}

MainWnd::~MainWnd()
{
}

void MainWnd::RegisterObserver(MainWndCallback *callback)
{
	callback_ = callback;
}

bool MainWnd::Create()
{
	if (!RegisterWindowClass())
	{
		return false;
	}

	wnd_ = ::CreateWindowExW(
		WS_EX_OVERLAPPEDWINDOW,
		kClassName,
		L"WebRTC",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	CreateChildWindows();
	SwitchToConnectUI();

	return wnd_ != NULL;
}

void MainWnd::SwitchToConnectUI()
{
	LayoutPeerListUI(false);
	ui_ = CONNECT_TO_SERVER;
	LayoutConnectUI(true);
}

LRESULT MainWnd::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

bool MainWnd::RegisterWindowClass()
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = kClassName;
	wnd_class_ = RegisterClass(&wc);
	return wnd_class_ != 0;
}

void MainWnd::CreateChildWindow(HWND *wnd, ChildWindowID id, const wchar_t *class_name, DWORD control_style, DWORD ex_style)
{
	DWORD style = WS_CHILD | control_style;
	*wnd = ::CreateWindowExW(
		ex_style, class_name, L"", style,
		100, 100, 100, 100,
		wnd_, reinterpret_cast<HMENU>(id), GetModuleHandle(NULL), NULL);
	::SendMessage(*wnd, WM_SETFONT, reinterpret_cast<WPARAM>(GetDefaultFont()), TRUE);
}

void MainWnd::CreateChildWindows()
{
	CreateChildWindow(&label1_, LABEL1_ID, L"Static", ES_CENTER | ES_READONLY, 0);
	CreateChildWindow(&edit1_, EDIT_ID, L"Edit", ES_LEFT | ES_NOHIDESEL | WS_TABSTOP, WS_EX_CLIENTEDGE);
	CreateChildWindow(&label2_, LABEL2_ID, L"Static", ES_CENTER | ES_READONLY, 0);
	CreateChildWindow(&edit2_, EDIT_ID, L"Edit", ES_LEFT | ES_NOHIDESEL | WS_TABSTOP, WS_EX_CLIENTEDGE);
	CreateChildWindow(&button_, BUTTON_ID, L"Button", BS_CENTER | WS_TABSTOP, 0);
	CreateChildWindow(&listbox_, LISTBOX_ID, L"ListBox", LBS_HASSTRINGS | LBS_NOTIFY, WS_EX_CLIENTEDGE);

	::SetWindowTextA(edit1_, server_.c_str());
	::SetWindowTextA(edit2_, port_.c_str());
}

void MainWnd::LayoutConnectUI(bool show)
{
	struct Windows
	{
		HWND wnd;
		const wchar_t *text;
		size_t width;
		size_t height;
	} windows[] = {
		{label1_, L"Server"},
		{edit1_, L"XXXyyyYYYgggXXXyyyYYYggg"},
		{label2_, L":"},
		{edit2_, L"XyXyX"},
		{button_, L"Connect"},
	};

	if (show)
	{
		const size_t kSeparator = 5;
		size_t total_width = (ARRAYSIZE(windows) - 1) * kSeparator;

		for (size_t i = 0; i < ARRAYSIZE(windows); ++i)
		{
			CalculateWindowSizeForText(windows[i].wnd, windows[i].text,
									   &windows[i].width, &windows[i].height);
			total_width += windows[i].width;
		}

		RECT rc;
		::GetClientRect(wnd_, &rc);
		size_t x = (rc.right / 2) - (total_width / 2);
		size_t y = rc.bottom / 2;
		for (size_t i = 0; i < ARRAYSIZE(windows); ++i)
		{
			size_t top = y - (windows[i].height / 2);
			::MoveWindow(windows[i].wnd, static_cast<int>(x), static_cast<int>(top),
						 static_cast<int>(windows[i].width),
						 static_cast<int>(windows[i].height), TRUE);
			x += kSeparator + windows[i].width;
			if (windows[i].text[0] != 'X')
				::SetWindowTextW(windows[i].wnd, windows[i].text);
			::ShowWindow(windows[i].wnd, SW_SHOWNA);
		}
	}
	else
	{
		for (size_t i = 0; i < ARRAYSIZE(windows); ++i)
		{
			::ShowWindow(windows[i].wnd, SW_HIDE);
		}
	}
}

void MainWnd::LayoutPeerListUI(bool show)
{
	if (show)
	{
		RECT rc;
		::GetClientRect(wnd_, &rc);
		::MoveWindow(listbox_, 0, 0, rc.right, rc.bottom, TRUE);
		::ShowWindow(listbox_, SW_SHOWNA);
	}
	else
	{
		::ShowWindow(listbox_, SW_HIDE);
		::InvalidateRect(wnd_, NULL, TRUE);
	}
}

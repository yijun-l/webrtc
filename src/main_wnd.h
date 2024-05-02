#pragma once

#include <string>
#include <Windows.h>
class MainWindow
{
public:
	virtual ~MainWindow() {}
	enum UI {
		CONNECT_TO_SERVER,
		LIST_PEERS,
		STREAMING,
	};
	virtual void SwitchToConnectUI() = 0;
};

class MainWnd : public MainWindow {
public:
	static const wchar_t kClassName[];
	MainWnd(const char* server, int port, bool auto_connect, bool auto_call);
	~MainWnd();
	bool Create();
	virtual void SwitchToConnectUI();

protected:
	enum ChildWindowID {
		EDIT_ID = 1,
		BUTTON_ID,
		LABEL1_ID,
		LABEL2_ID,
		LISTBOX_ID,
	};
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static bool RegisterWindowClass();

	void CreateChildWindow(HWND* wnd, ChildWindowID id, const wchar_t* class_name, DWORD control_style, DWORD ex_style);
	void CreateChildWindows();

	void LayoutConnectUI(bool show);
	void LayoutPeerListUI(bool show);

private:
	UI ui_;
	HWND wnd_;
	HWND edit1_;
	HWND edit2_;
	HWND label1_;
	HWND label2_;
	HWND button_;
	HWND listbox_;
	static ATOM wnd_class_;
	std::string server_;
	std::string port_;
	bool auto_connect_;
	bool auto_call_;
};
#include<Windows.h>
#include<sstream>
LRESULT WINAPI MyMsgPump(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
	case WM_CLOSE:
		PostQuitMessage(10);
		break;
	}
	return DefWindowProc(handle, msgcode, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpcmd, int cmdshow)
{
	constexpr auto classname = "ks_class";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MyMsgPump;
	wc.hInstance = hinstance;
	wc.style = CS_OWNDC;
	wc.lpszClassName = classname;
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindowEx(0, classname, "KS_dx_Window", WS_CAPTION|WS_SYSMENU| WS_MINIMIZEBOX | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, nullptr,nullptr,hinstance, nullptr);
	ShowWindow(hwnd, SW_SHOW);
	MSG msg;
	BOOL cptrd ;
	//getmessage returns 0 on quitmessage else -1 if any error occurs
	while (cptrd = GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (cptrd == -1)
	{
		return cptrd;
	}
	else
	{
     	return msg.wParam;
	}
}
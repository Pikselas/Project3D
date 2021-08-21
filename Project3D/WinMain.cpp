#include<Windows.h>
int WinMain(HINSTANCE hinstance, HINSTANCE hprev, LPSTR lpcmd, int cmdshow)
{
	constexpr auto classNm = "ks_cls";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = classNm;
	wc.hInstance = hinstance;
	wc.style = CS_OWNDC;
	RegisterClassEx(&wc);

	HWND handle = CreateWindowEx(0, classNm, "KS_DX_wnd", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 800, nullptr, nullptr, hinstance, nullptr);
	ShowWindow(handle, SW_SHOW);

	return 0;
}
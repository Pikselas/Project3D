#include<Windows.h>

LRESULT WINAPI customMsgPmp(HWND hndl, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
	case WM_CLOSE:
		PostQuitMessage(10);
		break;
	}
	return DefWindowProc(hndl, msgcode, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprev, LPSTR lpcmd, int cmdshow)
{
	constexpr auto classNm = "ks_cls";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = customMsgPmp;
	wc.lpszClassName = classNm;
	wc.hInstance = hinstance;
	wc.style = CS_OWNDC;
	RegisterClassEx(&wc);

	HWND handle = CreateWindowEx(0, classNm, "KS_DX_wnd", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 800, nullptr, nullptr, hinstance, nullptr);
	ShowWindow(handle, SW_SHOW);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
	}

	return msg.wParam;
}
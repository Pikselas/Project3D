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
	return 0;
}
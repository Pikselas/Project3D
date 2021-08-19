#include<Windows.h>
#include<sstream>
#include<d3d11.h>
#include<wrl.h>
#pragma comment(lib,"d3d11.lib")
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
	HWND hwnd = CreateWindowEx(0, classname, "KS_dx_Window", WS_CAPTION|WS_SYSMENU| WS_MINIMIZEBOX | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800, nullptr,nullptr,hinstance, nullptr);
	ShowWindow(hwnd, SW_SHOW);
	
	using Microsoft::WRL::ComPtr;
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;

	DXGI_SWAP_CHAIN_DESC scd = { 0 };

	scd.BufferCount = 1;
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.Flags = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &SwapChain, &Device, nullptr
		, &Context);
    
	ComPtr<ID3D11Resource> BackBuffer;
	SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &BackBuffer);
	ComPtr<ID3D11RenderTargetView> Target;
	Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &Target);
	
	MSG msg;
	//getmessage returns 0 on quitmessage else -1 if any error occurs
	float color[] = { 0.1f,1.1f,1.1f,0.0f };
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
		SwapChain->Present(1u, 0u);
		Context->ClearRenderTargetView(Target.Get(), color);
		
	}
	msg.wParam;
}
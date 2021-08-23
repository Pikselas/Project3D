#include<Windows.h>
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<sstream>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
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

	using Microsoft::WRL::ComPtr;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapchain;

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.Flags = 0u;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = handle;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0u, nullptr, 0u, D3D11_SDK_VERSION, &scd, &swapchain,
		&device, nullptr, &context);

	ComPtr<ID3D11RenderTargetView> target;
	ComPtr<ID3D11Resource> backBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target);

	struct vertex
	{
		float x, y;
	};
	vertex vertices[] = {
		{0.0f,0.5f},
		{0.5f,-0.5f},
		{-0.5f,-0.5f}
	};
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.ByteWidth = sizeof(vertices);
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdesc.CPUAccessFlags = 0u;
	bdesc.MiscFlags = 0u;
	bdesc.StructureByteStride = sizeof(vertex);

	D3D11_SUBRESOURCE_DATA sbrd = { 0 };
	sbrd.pSysMem = vertices;

	ComPtr<ID3D11Buffer> vertexBuffer;
	device->CreateBuffer(&bdesc, &sbrd, &vertexBuffer);
	UINT stride = sizeof(vertex);
	UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	ComPtr<ID3DBlob> blobData;

	ComPtr<ID3D11VertexShader> vShader;

	D3DReadFileToBlob(L"VertexShader1.cso", &blobData);
	device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), nullptr, &vShader);
	context->VSSetShader(vShader.Get(),nullptr,0u);
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION",0u,DXGI_FORMAT_R32G32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	ComPtr<ID3D11InputLayout> inpl;
	device->CreateInputLayout(ied, 1u, blobData->GetBufferPointer(), blobData->GetBufferSize(), &inpl);
	context->IASetInputLayout(inpl.Get());

	D3DReadFileToBlob(L"PixelShader1.cso", &blobData);
	ComPtr<ID3D11PixelShader> pShader;
	device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), nullptr, &pShader);
	context->PSSetShader(pShader.Get(), nullptr, 0u);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vp = { 0 };
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 800;
	vp.MinDepth = 0u;
	vp.MaxDepth = 1u;
	context->RSSetViewports(1u, &vp);


	float col[] = { 1.0f,1.2f,0.0f,1.0f };
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
		swapchain->Present(1u, 0u);
		context->ClearRenderTargetView(target.Get(), col);
		context->Draw((UINT)std::size(vertices), 0u);
	}

	return msg.wParam;
}
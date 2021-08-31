#include<Windows.h>
#include<d3d11.h>
#include<wrl.h>
#include<d3dcompiler.h>
#include<sstream>
#include<chrono>
#include<DirectXMath.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

float ptX = 0, ptY = 0;
LRESULT _stdcall WndProc(HWND handle, UINT msgCode, WPARAM wparam, LPARAM lparam)
{
    if (msgCode == WM_CLOSE)
    {
        PostQuitMessage(0);
    }
    else if (msgCode == WM_MOUSEMOVE)
    {
        POINTS pt = MAKEPOINTS(lparam);
        ptX = (float)(pt.x - 500) / 1000 * 2 ;
        ptY = (float)(pt.y - 425) / 850 * 2;
        std::ostringstream ss;
        ss << ptX << "," << ptY;
        SetWindowText(handle, ss.str().c_str());
    }
    return DefWindowProc(handle, msgCode, wparam, lparam);
}
int _stdcall WinMain(HINSTANCE hinstance, HINSTANCE hPrev, LPSTR lpcmd, int cmdshow)
{

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.hInstance = hinstance;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "Ks_Class";
    RegisterClassEx(&wc);
    HWND handle = CreateWindowEx(0, "Ks_Class", "_|KashiKichi|_", WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 850, nullptr, nullptr, hinstance, nullptr);
    ShowWindow(handle, SW_SHOW);
    MSG msg;
    using namespace Microsoft::WRL;
    ComPtr<ID3D11Device> graphics;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> target;
    DXGI_SWAP_CHAIN_DESC scd = { 0 };
    scd.BufferCount = 1;
    scd.BufferDesc.Width = 0;
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.Flags = 0u;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = handle;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0u, nullptr, 0u, D3D11_SDK_VERSION, &scd, &swapchain, &graphics, nullptr, &context);
    ComPtr<ID3D11Resource> backBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    graphics->CreateRenderTargetView(backBuffer.Get(), nullptr, &target);

    struct vertex
    {
        float x, y;
        unsigned char r, g, b, a;
    };
    vertex vertices[] = {
        {0.0f,0.5f,255,0,0},
        {0.5f,-0.5f,0,255,0},
        {-0.5f,-0.5f,0,0,255},
        
        {-0.3f,0.3f,0,255,0},
        {0.3f,0.3f,0,255,0},
        {0.0f,-0.8,255,0,0}
    };
    D3D11_BUFFER_DESC bd = { 0 };
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.StructureByteStride = sizeof(vertex);
    D3D11_SUBRESOURCE_DATA sd = { 0 };
    sd.pSysMem = vertices;
    ComPtr<ID3D11Buffer> VBuffer;
    graphics->CreateBuffer(&bd, &sd, &VBuffer);
    UINT stride = sizeof(vertex);
    UINT offset = 0u;
    context->IASetVertexBuffers(0u, 1u, VBuffer.GetAddressOf(), &stride, &offset);

    const unsigned short indices[] = {
        0,1,2,
        2,3,0,
        2,1,5,
        0,4,1
    };

    ComPtr<ID3D11Buffer> indexBuff;
    D3D11_BUFFER_DESC idb = { 0 };
    idb.ByteWidth = sizeof(indices);
    idb.Usage = D3D11_USAGE_DEFAULT;
    idb.BindFlags = D3D11_BIND_INDEX_BUFFER;
    idb.CPUAccessFlags = 0u;
    idb.MiscFlags = 0u;
    idb.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA isdb = { 0 };
    isdb.pSysMem = indices;

    graphics->CreateBuffer(&idb, &isdb, &indexBuff);
    context->IASetIndexBuffer(indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0u);

    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
    std::chrono::duration<float> dp;

    ComPtr<ID3D11VertexShader> vS;
    ComPtr<ID3DBlob> blb;
    D3DReadFileToBlob(L"VertexShader.cso", &blb);
    graphics->CreateVertexShader(blb->GetBufferPointer(), blb->GetBufferSize(), nullptr, &vS);
    context->VSSetShader(vS.Get(), nullptr, 0u);

    ComPtr<ID3D11InputLayout> inpl;
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
    };

    graphics->CreateInputLayout(ied, (UINT)std::size(ied), blb->GetBufferPointer(), blb->GetBufferSize(), &inpl);
    context->IASetInputLayout(inpl.Get());

    ComPtr<ID3D11PixelShader> ps;
    D3DReadFileToBlob(L"PixelShader.cso", &blb);
    graphics->CreatePixelShader(blb->GetBufferPointer(), blb->GetBufferSize(), nullptr, &ps);
    context->PSSetShader(ps.Get(), nullptr, 0u);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);
    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = 1000;
    vp.Height = 850;
    vp.MaxDepth = 1;
    vp.MinDepth = 0;
    context->RSSetViewports(1u, &vp);
    using namespace DirectX;
    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        dp = std::chrono::steady_clock::now() - tp;
        float angle = dp.count();

        struct constBuffer
        {
            XMMATRIX xmcb;
        };

        const constBuffer cbf =
        {
            {  
                 XMMatrixTranspose(XMMatrixRotationZ(angle) * XMMatrixScaling(20/17,1.0f,1.0f))
            }
        };

        ComPtr<ID3D11Buffer> constBuff;
        D3D11_BUFFER_DESC cbd = { 0 };
        cbd.ByteWidth = sizeof(cbf);
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA cbsrd = { 0 };
        cbsrd.pSysMem = &cbf;
        graphics->CreateBuffer(&cbd, &cbsrd, &constBuff);
        context->VSSetConstantBuffers(0u, 1u, constBuff.GetAddressOf());

        swapchain->Present(1u, 0u);
        context->ClearRenderTargetView(target.Get(), color);
        context->DrawIndexed((UINT)std::size(indices), 0u, 0);
    }
    return 0;
}
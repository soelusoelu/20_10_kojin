#include "DirectX.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderTargetView.h"
#include "Texture2D.h"
#include "../System/GlobalFunction.h"
#include "../System/Window.h"

namespace MyDirectX {

DirectX::DirectX() :
    mDevice(nullptr),
    mDeviceContext(nullptr),
    mDXGIFactory(nullptr),
    mSwapChain(nullptr),
    mDepthStencilView(nullptr),
    mRenderTargetView(nullptr),
    mDebugRenderTargetView(nullptr),
    mBlendState(nullptr),
    mDepthStencilState(nullptr),
    mRasterizerState(nullptr) {
}

DirectX::~DirectX() = default;

DirectX& DirectX::instance() {
    if (!mInstance) {
        mInstance = new DirectX();
    }
    return *mInstance;
}

void DirectX::initialize(const HWND& hWnd) {
    createDevice();
    createSwapChain(hWnd);
    createDepthStencilView();
    createRenderTargetView();
    setRenderTarget();
    setViewport(Window::width(), Window::height());

    mBlendState = std::make_shared<BlendState>();
    mDepthStencilState = std::make_shared<DepthStencilState>();
    mRasterizerState = std::make_shared<RasterizerState>();
}

void DirectX::finalize() {
    safeDelete(mInstance);
}

ID3D11Device* DirectX::device() const {
    return mDevice.Get();
}

ID3D11DeviceContext* DirectX::deviceContext() const {
    return mDeviceContext.Get();
}

ID3D11DepthStencilView* DirectX::depthStencilView() const {
    return mDepthStencilView.Get();
}

const std::shared_ptr<BlendState>& DirectX::blendState() const {
    return mBlendState;
}

const std::shared_ptr<DepthStencilState>& DirectX::depthStencilState() const {
    return mDepthStencilState;
}

const std::shared_ptr<RasterizerState>& DirectX::rasterizerState() const {
    return mRasterizerState;
}

void DirectX::setViewport(float width, float height, float x, float y) const {
    //ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = x;
    vp.TopLeftY = y;
    mDeviceContext->RSSetViewports(1, &vp);
}

void DirectX::setRenderTarget() const {
    mRenderTargetView->setRenderTarget();
}

void DirectX::setDebugRenderTarget() const {
    mDebugRenderTargetView->setRenderTarget();
}

void DirectX::setPrimitive(PrimitiveType primitive) const {
    mDeviceContext->IASetPrimitiveTopology(toPrimitiveMode(primitive));
}

void DirectX::draw(unsigned numVertex, unsigned start) {
    mDeviceContext->Draw(numVertex, start);
}

void DirectX::drawIndexed(unsigned numIndices, unsigned startIndex, int startVertex) {
    mDeviceContext->DrawIndexed(numIndices, startIndex, startVertex);
}

void DirectX::clearRenderTarget(float r, float g, float b, float a) const {
    mRenderTargetView->clearRenderTarget(r, g, b, a);
    mDebugRenderTargetView->clearRenderTarget(0.1f, 0.1f, 0.1f, 1.f);
}

void DirectX::clearDepthStencilView(bool depth, bool stencil) {
    unsigned mask = 0;
    if (depth) {
        mask |= D3D11_CLEAR_DEPTH;
    }
    if (stencil) {
        mask |= D3D11_CLEAR_STENCIL;
    }
    mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), mask, 1.f, 0); //深度バッファクリア
}

void DirectX::present() {
    mSwapChain->Present(0, 0);
}

void DirectX::createDevice() {
    //DXGIファクトリーの生成
    CreateDXGIFactory1(IID_PPV_ARGS(&mDXGIFactory));
    //アダプターの列挙用
    std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters;
    //ここに特定の名前を持つアダプターオブジェクトが入る
    Microsoft::WRL::ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
    for (int i = 0; mDXGIFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
        adapters.emplace_back(tmpAdapter);
    }

    //使用するアダプターを決定する
    for (int i = 0; i < adapters.size(); i++) {
        DXGI_ADAPTER_DESC adesc{};
        adapters[i]->GetDesc(&adesc); //アダプターの情報を取得
        std::wstring strDesc = adesc.Description; //アダプター名
        // Microsoft Basic Render Driver を回避
        if (strDesc.find(L"Microsoft") == std::wstring::npos && strDesc.find(L"Intel") == std::string::npos) {
            tmpAdapter = adapters[i]; //採用
            break;
        }
    }

    //対応レベルの配列
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    unsigned flags = 0;
#ifdef _DEBUG
    //デバッグ時はフラグを立てる
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    //採用したアダプターでデバイスを生成
    //D3D11CreateDevice(tmpAdapter.Get(), D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, _countof(levels), D3D11_SDK_VERSION, &mDevice, nullptr, &mDeviceContext);
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, _countof(levels), D3D11_SDK_VERSION, &mDevice, nullptr, &mDeviceContext);
}

void DirectX::createSwapChain(const HWND& hWnd) {
    //使用可能なMSAAを取得
    DXGI_SAMPLE_DESC MSAA = { 0 };
    for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1) {
        UINT quality = 0;
        if (SUCCEEDED(mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &quality))) {
            if (quality > 0) {
                MSAA.Count = i;
                MSAA.Quality = quality - 1;
            }
        }
    }

    // デバイスとスワップチェーンの作成
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
#ifdef _DEBUG
    swapChainDesc.BufferDesc.Width = Window::debugWidth();
    swapChainDesc.BufferDesc.Height = Window::debugHeight();
#else
    swapChainDesc.BufferDesc.Width = Window::width();
    swapChainDesc.BufferDesc.Height = Window::height();
#endif // _DEBUG
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    //swapChainDesc.SampleDesc = MSAA;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain = nullptr;
    mDXGIFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, &swapchain);
    //生成したIDXGISwapChainのオブジェクトをIDXGISwapChain4に変換する
    swapchain.As(&mSwapChain);
}

void DirectX::createRenderTargetView() {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    auto tex = std::make_unique<Texture2D>(backBuffer);
    mRenderTargetView = std::make_unique<RenderTargetView>(*tex);

    mDebugRenderTargetView = std::make_unique<RenderTargetView>(*tex);
}

void DirectX::createDepthStencilView() {
    Texture2DDesc desc;
#ifdef _DEBUG
    desc.width = Window::debugWidth();
    desc.height = Window::debugHeight();
#else
    desc.width = Window::width();
    desc.height = Window::height();
#endif // _DEBUG
    desc.format = Format::FORMAT_D24_UNORM_S8_UINT;
    desc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);

    auto tex = std::make_unique<Texture2D>(desc);
    mDevice->CreateDepthStencilView(tex->texture2D(), nullptr, &mDepthStencilView);
}

D3D11_PRIMITIVE_TOPOLOGY DirectX::toPrimitiveMode(PrimitiveType primitive) const {
    static const D3D11_PRIMITIVE_TOPOLOGY primitiveModes[] = {
        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, //PRIMITIVE_TYPE_POINTLIST = 0
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST, //PRIMITIVE_TYPE_LINELIST = 1
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, //PRIMITIVE_TYPE_LINESTRIP = 2
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //PRIMITIVE_TYPE_TRIANGLE_LIST = 3
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP //PRIMITIVE_TYPE_TRIANGLE_STRIP = 4
    };
    return primitiveModes[static_cast<int>(primitive)];
}

}

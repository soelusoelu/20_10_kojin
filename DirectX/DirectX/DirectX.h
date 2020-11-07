#pragma once

#include "BufferDesc.h"
#include "SamplerDesc.h"
#include "SubResourceDesc.h"
#include "Texture2DDesc.h"
#include "../System/SystemInclude.h"
#include <dxgi1_6.h>
#include <memory>

#pragma comment(lib, "dxgi.lib")

enum class PrimitiveType {
    POINT_LIST,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_LIST,
    TRIANGLE_STRIP
};

class BlendState;
class DepthStencilState;
class RasterizerState;
class RenderTargetView;

namespace MyDirectX {

class DirectX {
private:
    DirectX();
public:
    ~DirectX();
    static DirectX& instance();
    void initialize(const HWND& hWnd);
    void finalize();

    ID3D11Device* device() const;
    ID3D11DeviceContext* deviceContext() const;
    const std::shared_ptr<BlendState>& blendState() const;
    const std::shared_ptr<DepthStencilState>& depthStencilState() const;
    const std::shared_ptr<RasterizerState>& rasterizerState() const;

    void setViewport(float width, float height, float x = 0.f, float y = 0.f) const;
    void setRenderTarget() const;
    void setDebugRenderTarget() const;
    void setRenderTargets(ID3D11RenderTargetView* targets[], unsigned numTargets) const;
    void setPrimitive(PrimitiveType primitive) const;

    void draw(unsigned numVertex, unsigned start = 0);
    void drawIndexed(unsigned numIndices, unsigned startIndex = 0, int startVertex = 0);

    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;
    void clearDepthStencilView(bool depth = true, bool stencil = false);
    void present();

private:
    DirectX(const DirectX&) = delete;
    DirectX& operator=(const DirectX&) = delete;

    void createDevice();
    void createSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive) const;

private:
    static inline DirectX* mInstance = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
    Microsoft::WRL::ComPtr<IDXGIFactory6> mDXGIFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

    std::unique_ptr<RenderTargetView> mRenderTargetView;
    std::unique_ptr<RenderTargetView> mDebugRenderTargetView;
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthStencilState;
    std::shared_ptr<RasterizerState> mRasterizerState;
};

}

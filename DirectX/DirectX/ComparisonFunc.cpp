#include "ComparisonFunc.h"

D3D11_COMPARISON_FUNC toComparisonFunc(ComparisonFunc func) {
    static constexpr D3D11_COMPARISON_FUNC funcs[]{
        D3D11_COMPARISON_NEVER,
        D3D11_COMPARISON_LESS,
        D3D11_COMPARISON_EQUAL,
        D3D11_COMPARISON_LESS_EQUAL,
        D3D11_COMPARISON_GREATER,
        D3D11_COMPARISON_NOT_EQUAL,
        D3D11_COMPARISON_GREATER_EQUAL,
        D3D11_COMPARISON_ALWAYS
    };
    return funcs[static_cast<unsigned>(func)];
}

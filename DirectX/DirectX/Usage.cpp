#include "Usage.h"

D3D11_USAGE toUsage(Usage usage) {
    static constexpr D3D11_USAGE usages[] {
        D3D11_USAGE_DEFAULT,
        D3D11_USAGE_IMMUTABLE,
        D3D11_USAGE_DYNAMIC,
        D3D11_USAGE_STAGING
    };

    return usages[static_cast<unsigned>(usage)];
}

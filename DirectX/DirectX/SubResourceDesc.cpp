#include "SubResourceDesc.h"

D3D11_SUBRESOURCE_DATA toSubResource(const SubResourceDesc& data) {
    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem = data.data;
    srd.SysMemPitch = data.pitch;
    srd.SysMemSlicePitch = data.slicePitch;

    return srd;
}

#include "pch.h"
#include "ColormapLaoder.h"
#include "resource.h"
#include <wtypes.h>


void* ColormapLaoder::LoadColorMap(std::string fontfilename)
{
    HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_FONTS10), RT_RCDATA);
    if (myResource == nullptr) {
        return nullptr;
    }
    HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
    if (myResourceData == nullptr) {
        return nullptr;
    }
    void* pMyBinaryData = ::LockResource(myResourceData);

    return nullptr;
}